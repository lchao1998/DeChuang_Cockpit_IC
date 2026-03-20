/**
* @file         player_base.hpp
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

#ifndef AUTOSERVICES_PLAYER_BASE_HPP
#define AUTOSERVICES_PLAYER_BASE_HPP

#include <memory>
#include <utility>
#include <functional>

#include "utils/type_define.hpp"

namespace AutoSoundService
{
    using StateCb = std::function<void(const SoundState&)>;

    class PlayerBase
    {
    public:

        explicit PlayerBase(const PlayerType &type, StateCb cb) : m_type{type}, m_stateChangeCb(std::move(cb)) {}
        virtual ~PlayerBase() = default;

        const std::string& getName() { return m_name; }
        void setName(const std::string& name) { m_name = name; }

        PlayerType type() const { return m_type; }

        virtual void play(const PlayParams& playParams) = 0;
        virtual void stop(const StopType& stopType) = 0;
        virtual bool isBusy() { return PlayState::IDLE != m_state.state; }


        typedef std::shared_ptr<PlayerBase> Ptr;

    protected:
        void updateState();

    protected:
        SoundState m_state;

    private:
        const PlayerType    m_type;
        std::string         m_name;
        StateCb             m_stateChangeCb;
    };
}


#endif //AUTOSERVICES_PLAYER_BASE_HPP
