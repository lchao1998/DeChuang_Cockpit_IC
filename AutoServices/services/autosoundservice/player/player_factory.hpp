/**
* @file         player_factory.hpp
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
#ifndef AUTOSERVICES_PLAYER_FACTORY_HPP
#define AUTOSERVICES_PLAYER_FACTORY_HPP

#include "Log/log.hpp"
#include "player/wav_player.hpp"

#include <string>
#include <functional>

namespace AutoSoundService
{
    class PlayerFactory final
    {
    public:
        PlayerFactory();
        ~PlayerFactory();

        static std::string GetPlayerNameByType(const PlayerType &type);

        void registerPlayer(const PlayerType &playerType, const std::function<void(const SoundState&)> &callback)
        {
            if(!getPlayer(playerType))
            {
                PlayerBase::Ptr player;
                switch (playerType) {
                    case WAV:
                        static_assert(std::is_base_of<PlayerBase, WavPlayer>::value, "PlayerType invalid");
                        player = std::make_shared<WavPlayer>(callback);
                        logDebug("[%s] Create WAVPlayer", __FUNCTION__);
                        break;
                    default:
                        logWarning("Unsupported PlayerType: %d", playerType);
                        break;
                }
                if (player)
                {
                    player->setName(PlayerFactory::GetPlayerNameByType(playerType));
                    this->addPlayer(player);
                }
            }
        }

        PlayerBase::Ptr getPlayer(const PlayerType& playerType);

    private:
        void addPlayer(const PlayerBase::Ptr& player);

    private:
        std::unique_ptr<struct PlayerFactoryPrivate> m_p;
    };
}




#endif //AUTOSERVICES_PLAYER_FACTORY_HPP
