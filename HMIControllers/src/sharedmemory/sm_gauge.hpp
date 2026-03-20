/**
* @file         sm_gauge.hpp
* @brief        shared memory of ipc link data
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

#include "sharedmemory.hpp"
#include "sm_struct_define.h"

using namespace SharedMemory;

namespace SharedMemory {

    SM_POOL_DEFINITION(SharedMemoryPool_Gauge);

    SM_NUMBER_DEFINITION(int, gauge_VCU_VehVolt); //车辆总电压
    SM_NUMBER_DEFINITION(int, gauge_VCU_VehSOC); //车辆荷电状态
    SM_NUMBER_DEFINITION(int, gauge_VCU_TotalDistanceRange); //总里程
    SM_NUMBER_DEFINITION(int, gauge_SubTotalDistanceRange); //小计里程
    SM_NUMBER_DEFINITION(int, gauge_VCU_DistanceRange); //整车续驶里程
    SM_NUMBER_DEFINITION(int, gauge_VCU_VehSpd); //车速
    SM_NUMBER_DEFINITION(int, gauge_VCU_VehShiftLevel); //整车挡位
    SM_NUMBER_DEFINITION(int, gauge_VCU_MotPwrRate); //电机功率百分比
    SM_NUMBER_DEFINITION(int, gauge_VCU_ParkBrkSW); //驻车开关
    SM_NUMBER_DEFINITION(int, gauge_VCU_CCActive); //巡航控制激活
    SM_NUMBER_DEFINITION(int, gauge_VCU_CCEnableSW); //巡航控制系统启动开关
}

