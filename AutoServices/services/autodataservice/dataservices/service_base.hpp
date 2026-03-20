/**
* @file         service_base.hpp
* @brief        define service base class
* @details      register module
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
* <tr><td>2022/02/21  <td>v0.0.1   <td>          <td>Create
* </table>
*
**********************************************************************************
*/
#pragma  once 
#include "autosocm/application.hpp"

namespace AutoDataService
{
    class ServiceManager;

    class ServiceInterface
    {
    public:
        virtual ~ServiceInterface() {}
    };

    class ServiceBase
    {
    public:
        ServiceBase(ServiceInterface* manager, uint16_t service, uint16_t instance, const char* tag = NULL);
        virtual ~ServiceBase();

        std::shared_ptr<AutoSOCM::Service> getService();

    protected:
        const char* OUTPUT_TAG;
        ServiceInterface* m_service_manager;
        std::shared_ptr<AutoSOCM::Service> m_Service;
    };
}
