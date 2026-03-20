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
#ifndef AUTOSERVICES_SOUND_MANAGER_H
#define AUTOSERVICES_SOUND_MANAGER_H

#include "appfw/include/module.hpp"
#include "utils/type_define.hpp"

namespace AutoSoundService
{
    // this module name
    extern const std::string SOC_SOUND_SERVICE_MANAGER_NAME;

    class SoundManager final: public AppFw::Module
    {
    public:
        SoundManager();

        ~SoundManager() override;

        void reqPlaySnd(const SoundType &soundType, const PlayType &playType, uint16_t deadTimeMs, uint16_t repeatTimesOrDuration);

        void reqStopSnd(const StopType &stopType, const SoundType &soundType);

        void reqReset();

    private:
        void onReqPlaySnd(const PlayParams& params);
        void onReqStopSnd(const StopParams& params);

        void onReqReset();

    private:
        std::unique_ptr<struct SoundManagerPrivate> m_p;
    };
}


#endif // AUTOSERVICES_SOUND_MANAGER_H
