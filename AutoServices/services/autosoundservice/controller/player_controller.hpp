/**
* @file         player_controller.hpp
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
#ifndef AUTOSERVICES_PLAYER_CONTROLLER_HPP
#define AUTOSERVICES_PLAYER_CONTROLLER_HPP

#include "include/auto_sound_service.hpp"
#include "utils/type_define.hpp"

#include <memory>

namespace AutoSoundService
{
    class PlayerController final
    {
    public:
        explicit PlayerController();

        ~PlayerController();

        PlayerController(const PlayerController&) = delete;
        PlayerController &operator=(const PlayerController&) = delete;

        void start();

        void stop();

        void addTask(const PlayParams &playParams);

        void removeTask(const StopParams &stopParams);

        void reset();

    private:
        void playWorker();

        void onStateChange(const SoundState& state);

    private:
        std::unique_ptr<struct PlayerControllerPrivate> m_p;
    };
}

#endif //AUTOSERVICES_PLAYER_CONTROLLER_HPP
