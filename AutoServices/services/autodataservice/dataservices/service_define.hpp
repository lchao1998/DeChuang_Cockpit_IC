/**
* @file         service_define.hpp
* @brief        define service name and id
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
* <tr><td>2022/02/10  <td>v0.0.1   <td>          <td>Create
* </table>
*
**********************************************************************************
*/
#pragma once
#include <cstdint>
namespace AutoDataService
{
    // IPC service name
    extern const char*  IPC_Service_Name_Domain;
    extern const char*  IPC_Service_Name_Gauge;
    
    // IPC service ID
    extern const int32_t IPC_SERVICE_ID;

    // IPC instance ID
    extern const int32_t IPC_SERVICE_INSTANCE_GAUGEINFO;
    extern const int32_t IPC_SERVICE_INSTANCE_DOMAININFO;
}