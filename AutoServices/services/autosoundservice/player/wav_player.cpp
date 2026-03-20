#include "pcm.hpp"
#include "Log/log.hpp"
#include "wav_player.hpp"
#include "utils/type_define.hpp"

#include <map>
#include <array>
#include <chrono>
#include <atomic>
#include <numeric>
#include <sndfile.h>
#include <alsa/asoundlib.h>

namespace AutoSoundService
{
    struct WaveFilePrivate
    {
        std::string fileName;
        uint32_t    channels{0};
        uint32_t    bitPerSample{0};
        uint32_t    sampleRate{0};
        uint32_t    byteRate{0};
        uint32_t    bitRate{0};      // bit/s
        sf_count_t  frames{0};
        uint32_t    dataLen{0};
        uint32_t    duration{0};     // ms
        uint32_t    interval{0};     // ms
        u_char     *data{nullptr};
    };

    WaveFile::WaveFile()
        :   m_p(new WaveFilePrivate)
    {

    }

    WaveFile::~WaveFile() {
        delete []m_p->data;
        m_p->data = nullptr;
    }

    bool WaveFile::load(const char *fileName)
    {
        if (!fileName)
            return false;

        logDebug("WaveFile::parse fileName: %s", fileName);

        SNDFILE* infile = nullptr;
        SF_INFO inSFInfo;
        int majorFormat, subFormat, bytePerSample;

        memset(&inSFInfo, 0, sizeof(SF_INFO));
        if ((infile = sf_open(fileName, SFM_READ, &inSFInfo)) == nullptr)
        {
            logError("can't open sound file: %s", fileName);
            return false ;
        }
        logDebug("open sound file %s success.", fileName);

        majorFormat = inSFInfo.format & SF_FORMAT_TYPEMASK;
        subFormat   = inSFInfo.format & SF_FORMAT_SUBMASK;

        if(majorFormat != SF_FORMAT_WAV && majorFormat!= SF_FORMAT_WAVEX)
        {

            logError("current file isn't WAVE file, majorFormat: 0x%x", majorFormat);
            sf_close(infile);
            return false;
        }

        if(subFormat != SF_FORMAT_PCM_16 &&
           subFormat != SF_FORMAT_PCM_24 &&
           subFormat != SF_FORMAT_PCM_32)
        {
            sf_close(infile);
            return false;
        }

        bytePerSample = static_cast<int>(subFormat);
        size_t bufLen = inSFInfo.frames * inSFInfo.channels * bytePerSample;

        auto *buf = new u_char[bufLen]();

        sf_count_t readCount;
        readCount = sf_readf_short(infile, (short*)buf, inSFInfo.frames) ;

        if(readCount != inSFInfo.frames)
        {
            logError("got error while read wave data, readCount: %ld, in_sfinfo.frames: %ld", readCount, inSFInfo.frames);
            delete []buf;
            sf_close(infile);
            return false;
        }
        sf_close(infile);

        if (m_p->data)
        {
            delete []m_p->data;
            m_p->data = nullptr;
        }

        m_p->fileName      = fileName;
        m_p->channels      = inSFInfo.channels;
        m_p->bitPerSample  = bytePerSample * 8;
        m_p->data          = buf;
        m_p->dataLen       = bufLen;
        m_p->frames        = inSFInfo.frames;
        m_p->sampleRate    = inSFInfo.samplerate;
        m_p->byteRate      = m_p->sampleRate * m_p->channels * m_p->bitPerSample / 8;
        m_p->bitRate       = m_p->byteRate * 8;
        m_p->duration      = static_cast<uint32_t>(static_cast<double>(bufLen) / static_cast<double>(m_p->byteRate) * 1000);
        return true;
    }

    bool WaveFile::load(const std::vector<std::string> &files, uint32_t interval_ms)
    {
        if (!PCM::Instance().isOpened() || files.empty())
        {
            logError("the PCM is not opened, can't get the intervalDataLen");
            return false;
        }

        std::vector<int> audioDurations;
        std::vector<int> audioDateLens;

        /* update file info and cal total len */
        WaveFilePrivate fileInfo;
        for (auto &item : files)
        {
            WaveFile tmp;
            if (!tmp.load(item.c_str()))
            {
                logError("Can't load audio file: %s", item.c_str());
                return false;
            }
            fileInfo.fileName      += (item + ' ');
            fileInfo.channels       = std::max(fileInfo.channels, tmp.channels());
            fileInfo.bitPerSample   = std::max(fileInfo.bitPerSample, tmp.bitPerSample());
            fileInfo.frames        += tmp.frames();
            fileInfo.sampleRate     = std::max(fileInfo.sampleRate, tmp.sampleRate());
            fileInfo.byteRate       = std::max(fileInfo.byteRate, tmp.byteRate());
            fileInfo.bitRate        = std::max(fileInfo.bitRate, tmp.bitRate());

            audioDurations.push_back(static_cast<int>(tmp.duration()));
            audioDateLens.push_back(static_cast<int>(tmp.dataLength()));
        }
        int intervalDataLen         = static_cast<int>(PCM::Instance().millisecToBytes(interval_ms));
        int totalIntervalDataLen    = static_cast<int>(intervalDataLen * files.size());
        int totalIntervalMs         = static_cast<int>(interval_ms * files.size());
        int totalFileLen            = std::accumulate(audioDateLens.begin(), audioDateLens.end(), 0);
        int totalFileDuration       = std::accumulate(audioDurations.begin(), audioDurations.end(), 0);


        fileInfo.dataLen            = totalFileLen + std::max(totalIntervalDataLen - totalFileLen, 0);    // all interval len
        fileInfo.duration           = totalFileDuration + std::max(totalIntervalMs - totalFileDuration, 0);    // all interval time

        /* copy data */
        fileInfo.data = new u_char[fileInfo.dataLen]();
        if (!fileInfo.data)
        {
            logError("creat heap memory failed, size: %d", fileInfo.dataLen);
            return false;
        }
        size_t fileCount = files.size();
        u_char *currIdx = fileInfo.data;
        for (int i = 0; i < fileCount; ++i)
        {
            WaveFile tmp;
            if (!tmp.load(files.at(i).c_str()))
            {
                logError("Can't load audio file: %s", files.at(i).c_str());
                delete []fileInfo.data;
                return false;
            }
            memmove(currIdx, tmp.data(), tmp.dataLength());
            if (fileCount - 1 == i)
            {
                continue;
            }
            currIdx += (tmp.dataLength() + std::max(intervalDataLen - static_cast<int>(tmp.dataLength()), 0));
        }

        delete []m_p->data;
        m_p->fileName           = fileInfo.fileName;
        m_p->channels           = fileInfo.channels;
        m_p->bitPerSample       = fileInfo.bitPerSample;
        m_p->data               = fileInfo.data;
        m_p->dataLen            = fileInfo.dataLen;
        m_p->frames             = fileInfo.frames;
        m_p->sampleRate         = fileInfo.sampleRate;
        m_p->byteRate           = fileInfo.byteRate;
        m_p->bitRate            = fileInfo.bitRate;
        m_p->duration           = fileInfo.duration;
        m_p->interval           = interval_ms;
        return true;
    }

    void WaveFile::clear()
    {
        if (m_p->data)
        {
            delete []m_p->data;
            m_p->data = nullptr;
        }
        m_p->fileName.clear();

        m_p->channels      = 0;
        m_p->bitPerSample  = 0;
        m_p->dataLen       = 0;
        m_p->frames        = 0;
        m_p->sampleRate    = 0;
        m_p->byteRate      = 0;
        m_p->bitRate       = 0;
        m_p->duration      = 0;
    }

    u_char *WaveFile::data() const
    {
        return m_p->data;
    }

    uint32_t WaveFile::dataLength() const
    {
        return m_p->dataLen;
    }

    uint32_t WaveFile::frames() const
    {
        return m_p->frames;
    }

    uint32_t WaveFile::bitPerSample() const
    {
        return m_p->bitPerSample;
    }

    uint32_t WaveFile::sampleRate() const
    {
        return m_p->sampleRate;
    }

    uint32_t WaveFile::byteRate() const
    {
        return m_p->byteRate;
    }

    uint32_t WaveFile::bitRate() const
    {
        return m_p->bitRate;
    }

    uint32_t WaveFile::channels() const
    {
        return m_p->channels;
    }

    uint32_t WaveFile::duration() const
    {
        return m_p->duration;
    }

    uint32_t WaveFile::interval() const
    {
        return m_p->interval;
    }

    std::string WaveFile::info() const
    {
        return WaveFile::StringFormat(
                "-----------------------------------\n"
                "LibInfo        : %s\n"
                "FileName       : %s\n"
                "Channels       : %d\n"
                "BitPerSample   : %d\n"
                "DataLength     : %d\n"
                "NumberFrames   : %d\n"
                "SampleRate     : %d\n"
                "ByteRate       : %d\n"
                "BitRate        : %d\n"
                "Duration       : %d ms\n"
                "-----------------------------------\n",
                sf_version_string(),
                m_p->fileName.c_str(),
                m_p->channels,
                m_p->bitPerSample,
                m_p->dataLen,
                m_p->frames,
                m_p->sampleRate,
                m_p->byteRate,
                m_p->bitRate,
                m_p->duration
        );
    }

    struct WavContainerPrivate
    {
        std::map<SoundType, WaveFile::Ptr> wavMap;
    };

    WavContainer::WavContainer()
        :   m_p(new WavContainerPrivate)
    {

    }

    WavContainer::~WavContainer() = default;

    WaveFile::Ptr WavContainer::get(const PlayParams& playParams)
    {
        WaveFile::Ptr ret {nullptr};

        auto iter = m_p->wavMap.find(playParams.soundType);
        if(iter != m_p->wavMap.end() && iter->second->interval() == playParams.intervalMs)
        {
            ret = iter->second;
        }
        else
        {
            WaveFile::Ptr wavPtr = std::make_shared<WaveFile>();
            if (wavPtr->load(playParams.audioPaths, playParams.intervalMs))
            {
                m_p->wavMap.emplace(playParams.soundType, wavPtr);
                ret = wavPtr;
            }
        }
        return ret;
    }

    void WavContainer::add(const PlayParams& playParams)
    {
        auto iter = m_p->wavMap.find(playParams.soundType);
        if(iter == m_p->wavMap.end())
        {
            WaveFile::Ptr wavPtr = std::make_shared<WaveFile>();
            if (wavPtr->load(playParams.audioPaths, playParams.intervalMs))
                m_p->wavMap.emplace(playParams.soundType, wavPtr);
        }
    }

    size_t WavContainer::size()
    {
        return m_p->wavMap.size();
    }

    bool WavContainer::empty()
    {
        return m_p->wavMap.empty();
    }

    void WavContainer::clear()
    {
        m_p->wavMap.clear();
    }

    struct WavPlayerPrivate
    {
        WavContainer wavContainer;
        std::atomic<bool> stop {false};
        std::atomic<bool> stopImmediately {false};
    };

    WavPlayer::WavPlayer(const std::function<void(const SoundState&)>& cb)
        :   PlayerBase{PlayerType::WAV, cb},
            m_p{new WavPlayerPrivate}
    {
        PCM::Instance().open();
    }

    WavPlayer::~WavPlayer()
    {
        PCM::Instance().close();
    }

    void WavPlayer::play(const PlayParams& params)
    {
        logDebug("WavPlayer::play");
        if (this->isBusy())
        {
            logWarning("WavPlayer::play is busy!!!");
            usleep(1e5); // sleep 100ms
            return;
        }
        m_state.name = params.fileName;
        m_state.type = params.playType;
        m_state.remainTimesOrDuration = params.repeatTimesOrDuration;
        m_state.state = PlayState::PLAYING;
        m_p->stop = false;
        m_p->stopImmediately = false;
        this->updateState();
        this->playWorker(params);
    }

    void WavPlayer::stop(const StopType& stopType)
    {
        logDebug("WavPlayer::stop type = %d", (uint16_t)stopType);

        switch (stopType)
        {
            case StopType::FINISH_CYCLE:
            {
                m_p->stop = true;
            }break;
            case StopType::IMMEDIATELY:
            {
                m_p->stopImmediately = true;
            }break;
            default:
                break;
        }
    }

    void WavPlayer::playWorker(const PlayParams& playParams)
    {
        logDebug() << "WavPlayer::playWorker name = " << playParams.fileName << ", type = " << (uint16_t)playParams.playType
                   << ", repeatTimesOrDuration = " << playParams.repeatTimesOrDuration << ", deadTimeMs = " << playParams.intervalMs;

        auto wavPtr = m_p->wavContainer.get(playParams);

        if(nullptr == wavPtr || nullptr == wavPtr->data() || 0 == wavPtr->dataLength())
        {
            logWarning("[WavPlayer::playWorker]: get wav files failed! files: %s", playParams.fileName.c_str());
            m_state.state = PlayState::IDLE;
            updateState();
            usleep(2e5); // sleep for 200ms
            return;
        }

        uint32_t repeatTimes = getRepeatTimes(wavPtr, playParams.playType, playParams.repeatTimesOrDuration);
        uint32_t period = PCM::Instance().getPeriodSize();
        uint32_t wavLen = wavPtr->dataLength();
        uint32_t audiolen = wavLen;
        std::unique_ptr<uint8_t> audioBuf(new uint8_t[audiolen]());
        uint8_t periodBuf[period];
        uint32_t curIdx = 0;
        bool finish = false;
        memcpy(audioBuf.get(), wavPtr->data(), wavLen);

        std::chrono::time_point<std::chrono::high_resolution_clock> begin, end;
        begin = std::chrono::high_resolution_clock::now();

        PCM::Instance().prepare();

        while (repeatTimes && !finish)
        {
            memset(periodBuf, 0, period);
            uint32_t periodLeft = period;

            while (periodLeft > 0)
            {
                if (periodLeft <= audiolen - curIdx)
                {
                    memmove(periodBuf + (period - periodLeft), audioBuf.get() + curIdx, periodLeft);
#if 0
                    logDebug("Joe: 1 periodIdx = %d, audioIdx = %d, size = %d", (period - periodLeft), curIdx, periodLeft);
#endif
                    curIdx += periodLeft;
                    periodLeft = 0;
                }
                else
                {
                    if (1 == repeatTimes || m_p->stop)
                    {
                        if (curIdx < wavLen)
                        {
                            memmove(periodBuf + (period - periodLeft), audioBuf.get() + curIdx, wavLen - curIdx);
#if 0
                            logDebug("Joe: 1 periodIdx = %d, audioIdx = %d, size = %d", (period - periodLeft), curIdx, (wavLen - curIdx));
#endif
                            periodLeft -= (wavLen - curIdx);
                            curIdx = wavLen;
                        }
                        memset(periodBuf + (period - periodLeft), 0, periodLeft);
                        curIdx = 0;
                        periodLeft = 0;
                        repeatTimes = (std::numeric_limits<uint16_t>::max() == repeatTimes) ? std::numeric_limits<uint16_t>::max() : (repeatTimes - 1);
                        finish = true;
                    }
                    else
                    {
                        memmove(periodBuf + (period - periodLeft), audioBuf.get() + curIdx, audiolen - curIdx);
#if 0
                        logDebug("Joe: 1 periodIdx = %d, audioIdx = %d, size = %d", (period - periodLeft), curIdx, (audiolen - curIdx));
#endif
                        periodLeft -= (audiolen - curIdx);
                        curIdx = 0;
                        repeatTimes = (std::numeric_limits<uint16_t>::max() == repeatTimes) ? std::numeric_limits<uint16_t>::max() : (repeatTimes - 1);
                    }
                }
            }
            if (m_p->stopImmediately)
            {
                begin = std::chrono::high_resolution_clock::now();
                PCM::Instance().fadeOut(periodBuf, period, EFFECT_FRAMES_MAX);
                end = std::chrono::high_resolution_clock::now();
                logDebug("WavPlayer::playWorker stop Immediately time = %zu",
                         std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());
                finish = true;
            }
            if (!PCM::Instance().pcmWrite(periodBuf, period))
            {
                logError("[WavPlayer::playWorker] write PCM data failed!");
            }
        }
        PCM::Instance().drain();
        end = std::chrono::high_resolution_clock::now();
        logDebug("WavPlayer::WavPlayer time = %ld", std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count());

        m_state.remainTimesOrDuration = getRemainTimesOrDuration(wavPtr, playParams.playType, repeatTimes, playParams.intervalMs);
        m_state.state = PlayState::IDLE;
        this->updateState();
    }

    uint16_t WavPlayer::getRepeatTimes(const WaveFile::Ptr &wavPtr, PlayType playType, uint16_t repeatTimesOrDuration)
    {
        uint16_t ret = 0;

        if(!wavPtr)
            return ret;

        if(PlayType::TIMES == playType)
        {
            ret = repeatTimesOrDuration >= std::numeric_limits<uint16_t>::max() ? std::numeric_limits<uint16_t>::max() - 1 : repeatTimesOrDuration;
        }
        else if(PlayType::REPEAT == playType)
        {
            ret = std::numeric_limits<uint16_t>::max();
        }
        else if(PlayType::DURATION == playType)
        {
            ret = (repeatTimesOrDuration * 1000) / wavPtr->duration();
            if (!ret)
            {
                // make sure it will play at least once
                ret = 1;
            }
        }
        else
        {
            logError("Unsupported PlayType type: %d", static_cast<uint8_t>(playType));
        }

        return ret;
    }

    uint16_t WavPlayer::getRemainTimesOrDuration(const WaveFile::Ptr& wavPtr, PlayType playType, uint16_t repeatTimes, uint16_t deadTimeMs)
    {
        uint16_t ret = 0;

        if(!wavPtr)
            return ret;

        if (PlayType::TIMES == playType)
        {
            ret = repeatTimes;
        }
        else if (PlayType::REPEAT == playType)
        {
            ret = std::numeric_limits<uint16_t>::max();
        }
        else if(PlayType::DURATION == playType)
        {
            ret = (PCM::Instance().samplesToMillisec(wavPtr->frames()) + deadTimeMs) * repeatTimes;
        }
        else
        {
            logError("Unsupported PlayType type: %d", static_cast<uint8_t>(playType));
        }
        return ret;
    }
}