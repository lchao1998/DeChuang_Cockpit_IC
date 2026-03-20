#include "autosocm_define.hpp"

namespace AutoLogicController
{
    const char*     IPC_Service_Name_Warning            = "WarningService";
    const char*     IPC_Service_Name_Hardkey            = "HardkeyService";
    const char*     IPC_Service_Name_TRIPCOMPUTER       = "TripComputerService";
    const char*     IPC_Service_Name_Gauge              = "GaugeService";

    const int32_t IPC_SERVICE_ID                          = 0x2022;

    const int32_t IPC_SERVICE_INSTANCE_GAUGEINFO          = 0x0002;
    const int32_t IPC_SERVICE_INSTANCE_WARNINGINFO        = 0x0003;
    const int32_t IPC_SERVICE_INSTANCE_TRIPCOMPUTERINFO   = 0x0004;
    const int32_t IPC_SERVICE_INSTANCE_HARDKEYINFO        = 0x0005;
    const int32_t IPC_SERVICE_INSTANCE_DOMAININFO         = 0x0001;

    // sound service
    const int32_t IPC_SOUND_SERVICE_ID                    = 0x1005;
    const int32_t IPC_SERVICE_INSTANCE_SOUND              = 0x0001;
} 
