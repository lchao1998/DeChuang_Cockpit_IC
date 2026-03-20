#include "player_factory.hpp"
#include "utils/type_define.hpp"

#include <map>

namespace AutoSoundService
{

    struct PlayerFactoryPrivate
    {
        std::map<PlayerType, PlayerBase::Ptr> playerContainer;
    };

    PlayerFactory::PlayerFactory():
            m_p(new PlayerFactoryPrivate)
    {

    }
    PlayerFactory::~PlayerFactory() = default;

    PlayerBase::Ptr PlayerFactory::getPlayer(const PlayerType& playerType)
    {
        auto iter = m_p->playerContainer.find(playerType);
        if(iter != m_p->playerContainer.end())
            return iter->second;
        return nullptr;
    }

    void PlayerFactory::addPlayer(const PlayerBase::Ptr& player)
    {
        m_p->playerContainer[player->type()] = player;
    }

    std::string PlayerFactory::GetPlayerNameByType(const PlayerType &type)
    {
        std::string ret;

        switch (type)
        {
            case PlayerType::WAV :
                ret = "WavPlayer";
                break;
            default:
                break;
        }
        return ret;
    }
}