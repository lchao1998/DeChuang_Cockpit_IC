/**
* @file         moudle_define.hpp
* @brief        define moudles's name and so on
* @details      
* @author       xu.qiang@zlingsmart.com
* @date         2022-3-1
* @version      v0.0.1
* @copyright    Copyright (c) 2022, zlingsmart Corporation.
**********************************************************************************
* @attention 
* 
* @par modity log:
* <table>
* <tr><th>Date        <th>Version  <th>Author    <th>Description
* <tr><td>2022/03/01  <td>v0.0.1   <td>          <td>Create
* </table>
*
**********************************************************************************
*/
#pragma once
#include <cstdint>
#include "autosocm_define.hpp"
#include "log/log.hpp"

using namespace std;
using namespace AutoLogicController;

#define CHECK_IPC_VALUE_AND_SET_SHAREDMEMORY(client, sm, struc, type)           checkValue(client, sm##type, struc.type, #type)

namespace AutoLogicController
{
    // module name define
    extern const char* MODULE_NAME_COMMON;
    extern const char* MODULE_NAME_GAUGE;
    extern const char* MODULE_NAME_TELLTALE;
    extern const char* MODULE_NAME_TRIPCOMPUTER;
    extern const char* MODULE_NAME_WARNING;
    extern const char* MODULE_NAME_INTERACTION;
    extern const char* MODULE_NAME_CHIME;
    extern const char* MODULE_NAME_DOMAIN;

    // Enum :EPowerMode
    // Note :Power mode define
    enum EPowerMode
    {
      
    };
}
