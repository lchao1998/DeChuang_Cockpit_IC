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

    SM_POOL_DEFINITION(SharedMemoryPool_Domain);

    SM_NUMBER_DEFINITION(int, domain_VCU_VehErrInd); 
    SM_NUMBER_DEFINITION(int, domain_VCU_BMSErrInd); 
    SM_NUMBER_DEFINITION(int, domain_VCU_InsErrInd); 
    SM_NUMBER_DEFINITION(int, domain_VCU_DrvReadyInd); 
    SM_NUMBER_DEFINITION(int, domain_VCU_SOCLowInd ); 
    SM_NUMBER_DEFINITION(int, domain_VCU_BattChrgErrInd); 
    SM_NUMBER_DEFINITION(int, domain_VCU_TCUErrInd); 

    SM_NUMBER_DEFINITION(int, domain_VCU_VehSTOPInd); 
    SM_NUMBER_DEFINITION(int, domain_BCM_PosLgtStInd); 
    SM_NUMBER_DEFINITION(int, domain_BCM_FrontFogLightSts); 
    SM_NUMBER_DEFINITION(int, domain_BCM_LBeamSt); 
    SM_NUMBER_DEFINITION(int, domain_BCM_HBeamSt); 
    SM_NUMBER_DEFINITION(int, domain_BCM_RFogLgtSt); 
    SM_NUMBER_DEFINITION(float, domain_BCM_U_BATT); 

    SM_NUMBER_DEFINITION(int, domain_BCM_L_TurnLgtCmdToIC); 
    SM_NUMBER_DEFINITION(int, domain_BCM_R_TurnLgtCmdToIC); 
    SM_NUMBER_DEFINITION(int, domain_DCM_DriverDoorAjarSW); 
    SM_NUMBER_DEFINITION(int, domain_DCM_PassengerDoorAjarSW); 
    SM_NUMBER_DEFINITION(int, domain_DCM_DoorNotClosedInd); 

    SM_NUMBER_DEFINITION(int, domain_EPS_ErrLevel); 
}

