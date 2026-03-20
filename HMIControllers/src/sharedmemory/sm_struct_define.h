/**
* @file         sm_struct_define.h
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
#ifndef SM_STRUCT_DEFINE_H
#define SM_STRUCT_DEFINE_H


using namespace std;
#define SHARE_MEMORY_DEFAULT_SIZE (65535)

#define SM_POOL_DEFINITION(name) \
        extern SharedMemory::Pool name

#define SM_POOL_IMPLEMENTATION(name) \
        SharedMemory::Pool name(#name, SHARE_MEMORY_DEFAULT_SIZE);

#define SM_NUMBER_DEFINITION(type, name) \
        extern SharedMemory::AtomicNumber<type> name

#define SM_NUMBER_IMPLEMENTATION(type, name, pool, value) \
        SharedMemory::AtomicNumber<type> name(pool, #name, value)

#define SM_STRING_DEFINITION(name) \
        extern SharedMemory::AtomicString name

#define SM_STRING_IMPLEMENTATION(name, pool, value) \
        SharedMemory::AtomicString name(pool, #name, value)



enum EventDef {
	ICM_KEY_EVENT,
	AP_KEY_EVENT,
	CAN_INFO_CHANGE,
	ICM_INFO_CHANGE,
	ACC_CHANGE,
	IGH_CHANGE,
	VHAL_INFO_CHANGE = 10,
	EVENT_DEF_MAX
};
/**
* @enum KeyType
* @brief  define type of key
* @details
*/
enum KeyType {
	KEY_TYPE_OK = 0,
	KEY_TYPE_UP = 1,
	KEY_TYPE_DOWN = 2,
	KEY_TYPE_VIEW = 3
};
/**
* @enum KeyEvent
* @brief  define EVENT of key 
* @details
*/
enum KeyEvent {
	KEY_EVENT_UP = 0,
	KEY_EVENT_DOWN = 1,
	KEY_EVENT_SHORT = 2,
	KEY_EVENT_LONG = 3,
	KEY_EVENT_LONGLONG =4  //key event 
};

/**
* @struct SM_Common
* @brief  define shared memory data
* @details in memory
*/
struct SM_Common
{
    uint8_t ICM_PowerModeSt;
};


/**
* @struct SM_Gauge
* @brief  define shared memory data
* @details in memory
*/
struct SM_Gauge
{
    bool     Low_battery;
    bool     PowerOrChargeView;
    uint8_t  ICM_VehSpd;
    uint8_t  ICM_BattSOC;
    uint8_t  ICM_GearLvl;
    int8_t   ICM_VehPwr;
    uint8_t  ICM_PowerModeSt;
    uint64_t ICM_TotalOdometer; 
};



struct SM_Warning
{
    bool     ICM_PowerDoorWarn;
    bool     ICM_EmergencyPowerOffWarn;
    bool     ICM_PEPSWarn_161;
    bool     ICM_PEPSWarn_162;
    bool     ICM_PEPSWarn_163;
    bool     ICM_PEPSWarn_164;
    bool     ICM_PEPSWarn_165;
    bool     ICM_PEPSWarn_167;
    bool     ICM_PEPSWarn_168;
    bool     ICM_PEPSWarn_169;
    bool     ICM_PEPSWarn_1610;
    bool     ICM_PEPSWarn_1611;
    bool     ICM_SunRoofOpenWarn;
    bool     ICM_LampOnWarn;
    bool     ICM_WinOpenWarn;
    bool     ICM_DRLFaultSt;
    bool     ICM_EPBBrakingSt;
    bool     ICM_EPBChimeSt;
    bool     ICM_VehAvailDriveSt;
    bool     ICM_MotorTempWarn;
    bool     ICM_DCDCFaultSt;
    bool     ICM_ThermalRunawayErr;
    bool     ICM_12VPwrFaultWarn;
    bool     ICM_ThermalSysFaultWarn;
    bool     ICM_VehChgDischgSt;
    bool     ICM_ARBWarnNotice;
    uint8_t  ICM_ETCWarn;         
    uint8_t  ICM_LeftFrontTireSt;
    uint8_t  ICM_RightFrontTireSt;
    uint8_t  ICM_LeftRearTireSt;
    uint8_t  ICM_RightRearTireSt;
    uint8_t  ICM_ESCLWarn;     
    uint8_t  ICM_12VPowerLowWarn;  
    uint8_t  ICM_BackupLightFaultSt;
    uint8_t  ICM_BrakeLightFaultSt;
    uint8_t  ICM_TurnLightFaultSt;
    uint8_t  ICM_PositionFaultSt;
    uint8_t  ICM_RLSWarn;
    uint8_t  ICM_EPBDICCHAWarnSt;
    uint8_t  ICM_GearShiftNotice;
    uint8_t  ICM_SpdLimitNotice;
    uint8_t  ICM_PGearErrNotice;
    uint8_t  ICM_InslStChkMsgWarn;
    uint8_t  ICM_PrecResHotMsg;
    uint8_t  ICM_DispOverHeatWarn;
};
/**
* @struct SM_Telltale
* @brief  define shared memory data
* @details in memory
*/
struct SM_Telltale
{
    bool     ICM_Vehicle_DoorSt;
    bool     ICM_EPBAVHSt;
    bool     ICM_EPSWarn;
    bool     ICM_SOCLowSt;
    bool     ICM_ESPOffSt;  
    bool     ICM_SRSSt;
    bool     ICM_HighBeamSt;
    bool     ICM_ABSFaultSt;    
    bool     ICM_VehicleSysFaultSt;
    bool     ICM_DriverSysFaultSt;
    bool     ICM_BatterySysFaultSt;
    bool     ICM_ChargeSysFaultSt;
    bool     ICM_LimitPowerSt;      
    bool     ICM_ElectronicSysSt;
    bool     ICM_PlgConnSt;      
    uint8_t  ICM_DriverSeatbeltSt;
    uint8_t  ICM_DoorWarningSt;    
    uint8_t  ICM_TurnLightSt;     
    uint8_t  ICM_ESPFaultSt;     
    uint8_t  ICM_HDCSt;           
    uint8_t  ICM_EPBLightSt;     
    uint8_t  ICM_DeratePowerSt;
    uint8_t  ICM_IHBCSt;               
};

/**
* @struct SM_TripComputer
* @brief  define shared memory data
* @details in memory
*/
struct SM_TripComputer
{
    bool      ICM_VehRngStatus;           
    uint8_t   LF_TirePresssure;
    uint8_t   RF_TirePresssure;
    uint8_t   LB_TirePresssure;
    uint8_t   RB_TirePresssure;
    uint8_t   ICM_VehDrvMod;               
    uint8_t   ICM_VehChgDchgViewSt;       
    uint8_t   ICM_VehChgDchgDispTime_Min;
    uint8_t   ICM_VehChgDchgDispTime_hour;
    uint8_t   ICM_VehChgTargetPower;
    uint8_t   ICM_CharacterDisplay;
    uint8_t   ICM_MediaPlayTime;
    float     ICM_VehChgDchgCurrent;         
    float     ICM_VehChgDchgVoltage;        
    float     ICM_CorrectedExterTemp;       
    float     ICM_VehElcConspInst;
    float     ICM_VehElcConspAvg;
    float     ICM_VehTotalElcConspAvg;
    int16_t   ICM_LeftFrontTireTemperature;
    int16_t   ICM_RightFrontTireTemperature;
    int16_t   ICM_LeftRearTireTemperature;
    int16_t   ICM_RightRearTireTemperature;
    uint16_t  ICM_VehRng;                 
    uint16_t  ICM_VehElcConspAvg1Km;
    uint16_t  ICM_DriveTime;
    uint16_t  ICM_DriveDistance;

    uint8_t   ICM_MediaPlay_Percent;
    string    ICM_SongName;
    string    ICM_SingerName;
    string    ICM_SongCover;
    string    ICM_SongIyric;
   
};

struct SM_Interaction
{
    uint8_t    update_result;
    uint8_t    meter_value;  
};

#endif // SM_STRUCT_DEFINE_H