//
// Created by Administrator on 2022/5/24.
//

#include "player_base.hpp"

namespace AutoSoundService
{
    void PlayerBase::updateState()
    {
        if (m_stateChangeCb)
        {
            m_stateChangeCb(m_state);
        }
    }
}