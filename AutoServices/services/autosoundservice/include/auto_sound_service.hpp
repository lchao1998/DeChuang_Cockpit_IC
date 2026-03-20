/**
* @file         auto_sound_service.hpp
* @brief        define auto sound service base type
* @details      public header
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
#ifndef AUTOSERVICES_AUTO_SOUND_SERVICE_HPP
#define AUTOSERVICES_AUTO_SOUND_SERVICE_HPP

#include "AutoSOCM/include/data.hpp"

#include <cstdint>
#include <cassert>
#include <limits>

namespace AutoSoundService
{
    /**
     *  @struct SoundType
     *  @brief support sound type
     *  @details HMIController --> AutoSoundService
     */
    enum class SoundType: uint8_t
    {
        // sort by priority: low -> high
        /* begin */
        BUZZER_1 = 0,
        BUZZER_20,
        BUZZER_38,
        BUZZER_2,
        BUZZER_5,
        BUZZER_27,
        BUZZER_14,
        BUZZER_47,
        /* end */

        SIZE
    };

    /**
     *  @struct PlayType
     *  @brief
     *      TIMES: 按次数播放，小于int16_max
     *      REPEAT: 无限重复播放，无需指定重复次数或播放时长
     *      DURATION: 播放指定时长（若指定的播放时长小于音频文件的时长则至少播放一次），单位s，最大不超过65534s
     *  @details HMIController --> AutoSoundService
     */
    enum class PlayType : uint8_t
    {
        TIMES = 0,
        REPEAT,
        DURATION
    };

    /**
     *  @struct StopType
     *  @brief stop type
     *  @details HMIController --> AutoSoundService
     */
    enum class StopType : uint8_t
    {
        FINISH_CYCLE,
        IMMEDIATELY,
    };

    /**
     * @func  GetPlayParams
     * @param soundType
     * @param priority
     * @param type
     * @param repeatNumOrDuration
     * @param deadTimeMs
     * @return AutoSOCM::DataArray
     */
    inline AutoSOCM::DataArray GetPlayParams(const SoundType &soundType, const PlayType &playType,
                                             uint16_t interval_ms = 0, uint16_t repeatTimesOrDuration = 0)
    {
        AutoSOCM::DataArray parameter;
        assert(repeatTimesOrDuration != std::numeric_limits<uint16_t>::max());
        parameter.push_back(static_cast<uint8_t>(soundType));
        parameter.push_back(static_cast<uint8_t>(playType));
        parameter.push_back(interval_ms);
        parameter.push_back(repeatTimesOrDuration);
        return parameter;
    }

    /**
     * @func  GetStopParams
     * @param type
     * @return AutoSOCM::DataArray
     */
    inline AutoSOCM::DataArray GetStopParams(const StopType &type, const SoundType &soundType)
    {
        AutoSOCM::DataArray parameter;
        parameter.push_back(static_cast<uint8_t>(type));
        parameter.push_back(static_cast<uint8_t>(soundType));
        return parameter;
    }
}

#endif //AUTOSERVICES_AUTO_SOUND_SERVICE_HPP
