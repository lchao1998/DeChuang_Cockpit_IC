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
#ifndef AUTOSERVICES_SERVICE_BASE_HPP
#define AUTOSERVICES_SERVICE_BASE_HPP

#include "AutoSOCM/include/application.hpp"

namespace AutoSoundService
{
    class ServiceBase
    {

    public:
        explicit ServiceBase(uint16_t service, uint16_t instance, std::string tag);
        virtual ~ServiceBase();

    protected:
        const std::string                   m_serviceName;
        std::shared_ptr<AutoSOCM::Service>  m_service;
    };
}

#endif //AUTOSERVICES_SERVICE_BASE_HPP
