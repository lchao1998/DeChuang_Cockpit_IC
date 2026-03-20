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
#ifndef AUTOSERVICES_SERVICE_DEFINE_HPP
#define AUTOSERVICES_SERVICE_DEFINE_HPP

#include <string>

namespace AutoSoundService
{
    // IPC service name
    extern const std::string IPC_Service_Name_Sound;

    // IPC service ID
    extern const int32_t IPC_SERVICE_ID;

    // IPC instance ID
    extern const int32_t IPC_SERVICE_INSTANCE_SOUND;
}

#endif //AUTOSERVICES_SERVICE_DEFINE_HPP
