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
#ifndef AUTOSERVICES_SOUND_SERVICE_HPP
#define AUTOSERVICES_SOUND_SERVICE_HPP

#include "service_base.hpp"
#include "Log/log.hpp"

#include <map>
#include <boost/any.hpp>
#include <atomic>

namespace AutoSoundService
{
    class SoundService final : public ServiceBase
    {
    public:
        explicit SoundService();
        ~SoundService() override = default;

        void registerHandler(const std::string& name, const boost::any& func);

        template <typename T>
        std::function<T> getHandler(const std::string& name)
        {
            auto it = m_handlers.find(name);

            if (it == m_handlers.end())
            {
                return nullptr;
            }

            return boost::any_cast<std::function<T>>(it->second);
        }

        template <typename T, typename... Args>
        void executeHandler(const std::string& name, Args&&... args)
        {
            try
            {
                std::function<T> handle = getHandler<T>(name);
                if (handle != nullptr)
                {
                    (void)handle(std::forward<Args>(args)...);
                }
            } catch (const std::exception &e)
            {
                logError("exception thrown while getHandler<T>(name): %s", e.what());
            }
        }

    private:
        std::map<std::string, boost::any> m_handlers;
    };
}

#endif //AUTOSERVICES_SOUND_SERVICE_HPP
