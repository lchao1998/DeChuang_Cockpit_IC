#include "pcm.hpp"
#include "Log/log.hpp"

#include <alsa/asoundlib.h>


namespace AutoSoundService {


#define EFFECT_IS_FADE_IN(effect) ((effect)&FadeIn == FadeIn)
#define EFFECT_IS_FADE_OUT(effect) ((effect)&FadeOut == FadeOut)
#define MAKE_SILENCE(buf) memset(buf,0,sizeof(buf));

    struct PcmPrivate {
        uint                channels{0};
        snd_pcm_format_t    format{SND_PCM_FORMAT_UNKNOWN};
        uint                bitPerSample{0};
        uint                bitPerFrame{0};
        uint                rate{0};
        snd_pcm_uframes_t   chunkSize{0};
        uint                chunkBytes{0};
        EffectType          effect{EffectType::NoEffect};
        snd_pcm_t          *pcmHandle{nullptr};         //PCI设备句柄
    };

    PCM::PCM()
        : m_p(new PcmPrivate)
    {
        this->open();
    }

    PCM::~PCM() {
        this->close();
        delete m_p;
    }

    bool PCM::open(const char *device)
    {
        if (!m_p->pcmHandle)
        {
            int32_t rc;
            do
            {
                rc = snd_pcm_open(&m_p->pcmHandle, device, SND_PCM_STREAM_PLAYBACK, 0);
                if (rc < 0) {
                    logError("[PCM] open snd device:%s fail, return: %d", device, rc);
                }
                usleep(2e6); // sleep for 2s
            } while (rc < 0);
            this->setParams(PCM_DEFAULT_PARAMS);
        }
        return true;
    }

    bool PCM::isOpened()
    {
        return m_p->pcmHandle != nullptr;
    }

    void PCM::close() {
        if (m_p->pcmHandle) {
            snd_pcm_close(m_p->pcmHandle);
            m_p->pcmHandle = nullptr;
            //logDebug("[PCM] Close ALSA device: %s success", ALSA_DEVICE);
        }
    }

    void PCM::setParams(Params params) {
        //logDebug("[PCM] setParams[%d, %d, %d]", params.bitsPerSample, params.channels, params.rate);

        if (!m_p->pcmHandle)
            return;

        snd_pcm_hw_params_t *hwParams;
        snd_pcm_sw_params_t *swParams;
        snd_pcm_uframes_t buffer_size;
        snd_pcm_uframes_t period_size;
        uint32_t buffer_time = 0;
        uint32_t period_time = 0;

        int32_t err;

        snd_output_t *log = nullptr;

        snd_output_stdio_attach(&log, stdout, 0);

        snd_pcm_hw_params_alloca(&hwParams);
        snd_pcm_sw_params_alloca(&swParams);
        err = snd_pcm_hw_params_any(m_p->pcmHandle, hwParams);
        if (err < 0) {
            logWarning("[Pcm] Broken configuration for this PCM: no configurations available");
            return;
        }

        if (PCM_PARAMS_DUMP) {
            printf("HW Params of device \"%s\":\n", snd_pcm_name(m_p->pcmHandle));
            printf("--------------------\n");
            snd_pcm_hw_params_dump(hwParams, log);
            printf("--------------------\n");
        }

        err = snd_pcm_hw_params_set_access(m_p->pcmHandle, hwParams,
                                           SND_PCM_ACCESS_RW_INTERLEAVED);
        if (err < 0)
        {
            logWarning("[Pcm] Access type not available");
            return;
        }

        if (params.bitsPerSample != 16)
        {
            logWarning("[Pcm] Only accept 16bit format");
            return;
        }

        err = snd_pcm_hw_params_set_format(m_p->pcmHandle, hwParams, SND_PCM_FORMAT_S16_LE);
        if (err < 0)
        {
            logWarning("[Pcm] Sample format non available");
            return;
        }

        err = snd_pcm_hw_params_set_channels(m_p->pcmHandle, hwParams, params.channels);
        if (err < 0)
        {
            logWarning("[Pcm] Channels count non available");
            return;
        }

#if 0
        if (params.rate != 48000) {
            logWarning("[Pcm] Only accept 48000Hz rate");
            return;
        }
#endif
        uint32_t rate = params.rate;
        err = snd_pcm_hw_params_set_rate_near(m_p->pcmHandle, hwParams, &(params.rate), nullptr);
        assert(err >= 0);

        if ((float)rate * 1.05 < params.rate || (float)rate * 0.95 > params.rate) {
            char plugex[64];
            const char *pcmname = snd_pcm_name(m_p->pcmHandle);
            logInfo("Warning: rate is not accurate (requested = %iHz, got = %iHz)\n", rate, params.rate);
            if (! pcmname || strchr(snd_pcm_name(m_p->pcmHandle), ':'))
            {
                *plugex = 0;
            }
            else
            {
                snprintf(plugex, sizeof(plugex), "(-Dplug:%s)",
                         snd_pcm_name(m_p->pcmHandle));
            }
            logInfo("         please, try the plug plugin %s\n", plugex);
        }

        rate = params.rate;

        err = snd_pcm_hw_params_get_buffer_time_max(hwParams, &buffer_time, nullptr);
        assert(err >= 0);
        if (buffer_time > 500000)
            buffer_time = 500000;

        period_time = buffer_time / 4; //max 250ms
        logInfo("[Pcm] period_time = %d, buffer_time = %d", period_time, buffer_time);
        err = snd_pcm_hw_params_set_period_time_near(m_p->pcmHandle, hwParams, &period_time, nullptr);
        assert(err >= 0);

        err = snd_pcm_hw_params_set_buffer_time_near(m_p->pcmHandle, hwParams, &buffer_time, nullptr);
        assert(err >= 0);

        err = snd_pcm_hw_params(m_p->pcmHandle, hwParams);
        if (err < 0)
        {
            logWarning("[Pcm] Unable to install hw params:");
            snd_pcm_hw_params_dump(hwParams, log);
            return;
        }

        snd_pcm_hw_params_get_period_size(hwParams, &period_size, nullptr);
        snd_pcm_hw_params_get_buffer_size(hwParams, &buffer_size);
        if (period_size == buffer_size) {
            logWarning("[Pcm] Can't use period equal to buffer size (%lu == %lu)",
                  period_size, buffer_size);
            return;
        }

        err = snd_pcm_sw_params_current(m_p->pcmHandle, swParams);
        if (err < 0) {
            logWarning("Unable to get current sw params.");
            return;
        }

        err = snd_pcm_sw_params_set_avail_min(m_p->pcmHandle, swParams, period_size);

        if (snd_pcm_sw_params(m_p->pcmHandle, swParams) < 0)
        {
            logWarning("[Pcm] unable to install sw params:");
            snd_pcm_sw_params_dump(swParams, log);
            return;
        }

        if (PCM_PARAMS_DUMP)
            snd_pcm_dump(m_p->pcmHandle, log);

        snd_pcm_hw_params_get_format(hwParams, &(m_p->format));
        snd_pcm_hw_params_get_channels(hwParams, &(m_p->channels));
        snd_pcm_hw_params_get_period_size(hwParams, &(m_p->chunkSize), 0);
        snd_pcm_hw_params_get_rate(hwParams, &(m_p->rate), 0);
        m_p->bitPerSample = snd_pcm_format_physical_width(m_p->format);
        m_p->bitPerFrame = m_p->bitPerSample * m_p->channels;
        m_p->chunkBytes = m_p->chunkSize * m_p->bitPerFrame / 8;
        snd_pcm_prepare(m_p->pcmHandle);
        //logDebug("[Pcm] m_p->chunkBytes = %d", m_p->chunkBytes);
    }

    void PCM::setEffect(EffectType effect) {
        m_p->effect = effect;
    }

    uint32_t PCM::getPeriodSize() {
        return m_p->chunkBytes;
    }

    void PCM::prepare() {
        snd_pcm_prepare(m_p->pcmHandle);
    }

    void PCM::drain() {
        snd_pcm_drain(m_p->pcmHandle);
    }

    uint32_t PCM::pcmWrite(uint8_t *buf, uint32_t len) {
        if (!m_p->pcmHandle)
            return 0;

        uint32_t total = 0;
        snd_pcm_uframes_t frames = snd_pcm_bytes_to_frames(m_p->pcmHandle, len);
        while (frames > 0)
        {
            snd_pcm_sframes_t ret = snd_pcm_writei(m_p->pcmHandle, buf, frames);
//            logDebug("pcmWrite len: %d, write: %lu", len, frames);
            if (ret < 0) {
                snd_pcm_prepare(m_p->pcmHandle);
                logWarning() << "xrun,retry";
                continue;
            }

            uint32_t bytes = snd_pcm_frames_to_bytes(m_p->pcmHandle, ret);
            buf += bytes;
            frames -= ret;
            total += bytes;
        }
        return total;
    }

    void PCM::fadeIn(uint8_t *data, uint len, uint32_t max) {
        uint32_t framesCnt = len / (m_p->bitPerSample / 8) / m_p->channels;

        uint32_t effectSamplesCnt = framesCnt <= max ? framesCnt * m_p->channels : max * m_p->channels;

        if (!effectSamplesCnt)
            return;

        auto *samples = (int16_t *) data;

        for (uint32_t i = 0; i < effectSamplesCnt; ++i) {
            float s = samples[i];
            s = s * (i / (effectSamplesCnt - 1.0));
            samples[i] = s;
        }
    }

    void PCM::fadeOut(uint8_t *data, uint32_t len, uint32_t max) {
        uint32_t framesCnt = len / (m_p->bitPerSample / 8) / m_p->channels;

        uint32_t samplesCnt = framesCnt * m_p->channels;

        uint32_t effectSamplesCnt = framesCnt <= max ? framesCnt * m_p->channels : max * m_p->channels;

        if (!effectSamplesCnt)
            return;

        int16_t *samples = (int16_t *) data + (samplesCnt - effectSamplesCnt);

        for (uint32_t i = 0; i < effectSamplesCnt; ++i) {
            float s = samples[i];
            s = s * ((effectSamplesCnt - 1) - i) / (effectSamplesCnt - 1);
            samples[i] = s;
#if 0
            //logDebug("samples[%d] = %d", i, samples[i]);
#endif
        }
    }

    uint32_t PCM::millisecToSamples(uint32_t ms) {
        return m_p->rate * ms / 1000;
    }

    uint32_t PCM::millisecToBytes(uint32_t ms) {
        assert(m_p->pcmHandle != nullptr);

        uint32_t samples = millisecToSamples(ms * PCM_DEFAULT_CHANNEL);
        return snd_pcm_samples_to_bytes(m_p->pcmHandle, samples);
    }

    uint32_t PCM::millisecToFrames(uint32_t ms) {
        assert(m_p->pcmHandle != nullptr);
        uint32_t bytes = millisecToBytes(ms);
        return snd_pcm_bytes_to_frames(m_p->pcmHandle, bytes);
    }

    uint32_t PCM::bytesToMillisec(uint32_t bytes) {
        assert(m_p->pcmHandle != nullptr);
        uint32_t samples = snd_pcm_bytes_to_samples(m_p->pcmHandle, bytes);
        return samplesToMillisec(samples);
    }

    uint32_t PCM::samplesToMillisec(uint32_t samples) {
        return samples * 1000 / m_p->rate;
    }

    uint32_t PCM::framesToMillisec(uint32_t frames) {
        assert(m_p->pcmHandle != nullptr);
        uint32_t bytes = snd_pcm_frames_to_bytes(m_p->pcmHandle, frames);
        return bytesToMillisec(bytes);
    }
}
