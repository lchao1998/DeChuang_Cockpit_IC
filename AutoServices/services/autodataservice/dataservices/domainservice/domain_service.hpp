/**
* @file         domain_service.hpp
* @brief        domain service contains chime warning telltale info
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
    class DomainService : public ServiceBase
    {
    public:
        explicit DomainService(ServiceInterface* manager);
        virtual ~DomainService();

        void init();
    };

} 