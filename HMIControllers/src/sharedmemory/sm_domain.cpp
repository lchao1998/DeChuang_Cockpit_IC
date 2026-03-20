#include "sm_domain.hpp"

namespace SharedMemory
{
    SM_POOL_IMPLEMENTATION(SharedMemoryPool_Domain);

    SM_NUMBER_IMPLEMENTATION(int, domain_VCU_VehErrInd, SharedMemoryPool_Domain, 0); 
    SM_NUMBER_IMPLEMENTATION(int, domain_VCU_BMSErrInd, SharedMemoryPool_Domain, 0); 
    SM_NUMBER_IMPLEMENTATION(int, domain_VCU_InsErrInd, SharedMemoryPool_Domain, 0); 
    SM_NUMBER_IMPLEMENTATION(int, domain_VCU_DrvReadyInd, SharedMemoryPool_Domain, 0); 
    SM_NUMBER_IMPLEMENTATION(int, domain_VCU_SOCLowInd, SharedMemoryPool_Domain, 0); 
    SM_NUMBER_IMPLEMENTATION(int, domain_VCU_BattChrgErrInd, SharedMemoryPool_Domain, 0); 
    SM_NUMBER_IMPLEMENTATION(int, domain_VCU_TCUErrInd, SharedMemoryPool_Domain, 0); 

    SM_NUMBER_IMPLEMENTATION(int, domain_VCU_VehSTOPInd, SharedMemoryPool_Domain, 0); 
    SM_NUMBER_IMPLEMENTATION(int, domain_BCM_PosLgtStInd, SharedMemoryPool_Domain, 0); 
    SM_NUMBER_IMPLEMENTATION(int, domain_BCM_FrontFogLightSts, SharedMemoryPool_Domain, 0); 
    SM_NUMBER_IMPLEMENTATION(int, domain_BCM_LBeamSt, SharedMemoryPool_Domain, 0); 
    SM_NUMBER_IMPLEMENTATION(int, domain_BCM_HBeamSt, SharedMemoryPool_Domain, 0); 
    SM_NUMBER_IMPLEMENTATION(int, domain_BCM_RFogLgtSt, SharedMemoryPool_Domain, 0); 
    SM_NUMBER_IMPLEMENTATION(float, domain_BCM_U_BATT, SharedMemoryPool_Domain, 0); 

    SM_NUMBER_IMPLEMENTATION(int, domain_BCM_L_TurnLgtCmdToIC, SharedMemoryPool_Domain, 0); 
    SM_NUMBER_IMPLEMENTATION(int, domain_BCM_R_TurnLgtCmdToIC, SharedMemoryPool_Domain, 0); 
    SM_NUMBER_IMPLEMENTATION(int, domain_DCM_DriverDoorAjarSW, SharedMemoryPool_Domain, 0); 
    SM_NUMBER_IMPLEMENTATION(int, domain_DCM_PassengerDoorAjarSW, SharedMemoryPool_Domain, 0); 
    SM_NUMBER_IMPLEMENTATION(int, domain_DCM_DoorNotClosedInd, SharedMemoryPool_Domain, 0); 
    
    SM_NUMBER_IMPLEMENTATION(int, domain_EPS_ErrLevel, SharedMemoryPool_Domain, 0);
}

