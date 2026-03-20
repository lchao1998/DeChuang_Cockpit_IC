/**
* @file         gauge_service.hpp
* @brief        gauge service
* @details      IPC service
* @author       xu.qiang@zlingsmart.com
* @date         2022-2-21
* @version      v0.0.1
* @copyright    Copyright (c) 2022, zlingsmart Corporation.
**********************************************************************************
* @attention 
* 
* @par modity log:
* <table>
* <tr><th>Date        <th>Version  <th>Author    <th>Description
* <tr><td>2022/02/10  <td>v0.0.1   <td>          <td>Create
* </table>
*
**********************************************************************************
*/
#pragma  once

#include "service_base.hpp"

namespace AutoDataService
{
    class GaugeService : public ServiceBase
    {
    public:
        explicit GaugeService(ServiceInterface* manager);
        virtual ~GaugeService();

        void init();
    };

} 