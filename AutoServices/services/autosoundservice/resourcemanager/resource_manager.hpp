/**
* @file         resource_manager.hpp
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
#ifndef AUTOSERVICES_RESOURCE_MANAGER_HPP
#define AUTOSERVICES_RESOURCE_MANAGER_HPP

#include "AutoServices/services/autosoundservice/utils/type_define.hpp"

#include <map>

namespace AutoSoundService
{
    class ResourceManager final
    {
    public:
         explicit ResourceManager() = default;
        ~ResourceManager() = default;

        static AudioFiles GetAudioFilePath(const SoundType &soundType);

        static std::string SoundType2String(const SoundType & soundType);

        static PlayerType  GetPlayerType(const SoundType &soundType);

        static std::string GetPlayerName(const PlayerType & playerType);

        static size_t Size()
        {
            return SoundFileMaps.size();
        }

    private:
        static std::map<SoundType, AudioFiles> SoundFileMaps;
    };
}

#endif //AUTOSERVICES_RESOURCE_MANAGER_HPP
