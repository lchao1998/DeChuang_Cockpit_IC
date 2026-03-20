/**
* @file         wav_player.hpp
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
#ifndef AUTOSERVICES_SERVICE_TYPE_DEFINE_HPP
#define AUTOSERVICES_SERVICE_TYPE_DEFINE_HPP

#include "include/auto_sound_service.hpp"

#include <memory>

namespace AutoSoundService
{
#define PCM_DEFAULT_CHANNEL 2
    enum class PlayState : uint8_t
    {
        IDLE,
        PLAYING,
    };

    enum PlayerType : uint8_t
    {
        /* Begin */
        WAV = 0,
        /* End */

        Size
    };

    struct AudioFiles
    {
        std::vector<std::string> pathContainer;
        uint32_t                 interval_ms;
    };

    struct SoundState
    {
        std::string     name;
        uint            priority {0};
        PlayerType      playerType {PlayerType::WAV};
        PlayType        type {PlayType::TIMES};
        uint16_t        remainTimesOrDuration {0};
        PlayState       state {PlayState::IDLE};
    };

    struct PlayParams
    {
        SoundType                   soundType{SoundType::SIZE};
        PlayerType                  playerType {PlayerType::WAV};
        PlayType                    playType {PlayType::TIMES};
        uint32_t                    intervalMs {0}; // ms
        uint16_t                    repeatTimesOrDuration {0};
        std::string                 fileName;
        std::vector<std::string>    audioPaths;

        typedef std::shared_ptr<PlayParams> Ptr;
    };

    struct StopParams
    {
        StopType        stopType;
        SoundType       soundType;
    };
}

#endif //AUTOSERVICES_SERVICE_TYPE_DEFINE_HPP
