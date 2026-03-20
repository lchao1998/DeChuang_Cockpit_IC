/**
* @file         pcm.hpp
* @brief
* @details
* @author       long.xinzheng@zlingsmart.com
* @date         2022-6-9
* @version      v0.0.1
* @copyright    Copyright (c) 2022, zlingsmart Corporation.
**********************************************************************************
* @attention
*
* @par modify log:
* <table>
* <tr><th>Date        <th>Version  <th>Author    <th>Description
* <tr><td>2022/02/21  <td>v0.0.1   <td>          <td>Create
* </table>
*
**********************************************************************************
*/
#ifndef AUTOSERVICES_PCM_HPP
#define AUTOSERVICES_PCM_HPP

#include "utils/type_define.hpp"

#include <cstdint>

namespace AutoSoundService
{
#define PCM_DEFAULT_PARAMS { 16, PCM_DEFAULT_CHANNEL, 48000 } // 16bit 2 channel 48000 sample rate
#ifdef __x86_64__
#define ALSA_DEVICE "default"
#else
#define ALSA_DEVICE "default"
#endif
#define EFFECT_FRAMES_MIN 100
#define EFFECT_FRAMES_MAX 5000
constexpr bool PCM_PARAMS_DUMP = true;

    enum EffectType : uint8_t {
        NoEffect = 0x00,
        FadeIn   = 0x01,
        FadeOut  = 0x02
    };

    class PCM {
    public:
        PCM(const PCM&) = delete;
        PCM& operator=(const PCM&) = delete;
        static PCM &Instance()
        {
            static PCM instance;
            return instance;
        }

        /**
         * @brief The Params struct audio device params
         */
        struct Params {
            uint16_t bitsPerSample;
            uint16_t channels;
            uint32_t rate;
        };

        /**
         * @brief open  open audio device
         * @return success open return true, otherwise return false
         */
        bool open(const char *device = ALSA_DEVICE);

        bool isOpened();

        /**
         * @brief close close audio device,hw buffer will be clear immediately
         */
        void close();

        /**
         * @brief setParams set audio device params
         * @param params
         */
        void setParams(Params params);

        /**
         * @brief effect is FadeIn or FadeOut,
         *        i use FadeInOut effect to avoid the boom like ether at begin or end of the play sound
         *
         * @param effect type
         */
        void setEffect(EffectType effect);

        uint32_t getPeriodSize();

        void fadeIn(uint8_t *data, uint32_t len, uint32_t max);

        void fadeOut(uint8_t *data, uint32_t len, uint32_t max);

        //Helper function
        uint32_t millisecToBytes(uint32_t ms);

        uint32_t millisecToSamples(uint32_t ms);

        uint32_t millisecToFrames(uint32_t ms);

        uint32_t bytesToMillisec(uint32_t bytes);

        uint32_t samplesToMillisec(uint32_t samples);

        uint32_t framesToMillisec(uint32_t frames);

        //Write data to pcm buffer
        uint32_t pcmWrite(uint8_t *data, uint32_t len);

        void prepare();

        void drain();

    private:
        PCM();
        ~PCM();

        struct PcmPrivate  *m_p;
    };

}
#endif // AUTOSERVICES_PCM_HPP
