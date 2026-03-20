#pragma once 
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>


namespace AutoDataService {

    // CAN DATA LENGTH
    #define CAN_DATA_LENGTH 8

    // CAN ID
    #define CANID_VCU_VehInfoA 			(0x1CFF0227 & CAN_EFF_MASK)
    #define CANID_VCU_VehInfoB			(0x10FF0327 & CAN_EFF_MASK)
    #define CANID_VCU_MotST				(0x10FF8A27 & CAN_EFF_MASK)
    #define CANID_VCU_CCVS				(0x18FEF127 & CAN_EFF_MASK)
    #define CANID_VCU_DispInfo			(0x18FF0727 & CAN_EFF_MASK)
    #define CANID_BCM_CPD1				(0x18FDCD21 & CAN_EFF_MASK)
    #define CANID_BCM_CPD3				(0x18FFD921 & CAN_EFF_MASK)
    #define CANID_DCM_ConfigStatus		(0x18FDA5EC & CAN_EFF_MASK)
    #define CANID_EPS_ERROR             (0x18FFD013 & CAN_EFF_MASK)

    typedef union 
    {
        uint8_t data[8];
        struct 
        {
            uint8_t vehicleVoltageLow;
            uint8_t vehicleVoltageHigh;
            uint8_t reserve3;
            uint8_t reserve4;
            uint8_t vehicleSOC;
            uint8_t vehicleDistanceRangeLow;
            uint8_t vehicleDistanceRangeHigh;
            uint8_t reserve8;
        }signal;
    } CanSignalVCUVehInfoA;

    typedef union 
    {
        uint8_t data[8];
        struct 
        {
            uint8_t wheelBasedVehicleSpeedLow;
            uint8_t wheelBasedVehicleSpeedHigh;
            uint8_t vehicleShiftLevel;
            uint8_t reserve4;
            uint8_t reserve5;
            uint8_t reserve6;
            uint8_t reserve7;
            uint8_t reserve8;
        }signal;
    } CanSignalVCUVehInfoB;

    typedef union 
    {
        uint8_t data[8];
        struct 
        {
            uint8_t reserve1;
            uint8_t reserve2;
            uint8_t reserve3;
            uint8_t reserve4;
            uint8_t motorPowerRate;
            uint8_t reserve6;
            uint8_t reserve7;
            uint8_t reserve8;
        }signal;
    } CanSignalVCUMotST;

    typedef union 
    {
        uint8_t data[8];
        struct 
        {
            uint8_t reserve1_bit1_2:2;
            uint8_t parkingBrakeSwitch:2;
            uint8_t reserve1_bit5_8:4;
            uint8_t reserve2;
            uint8_t reserve3;
            uint8_t cruiseControlActive:2;
            uint8_t cruiseControlEnable:2;
            uint8_t reserve4_bit5_8:4;
            uint8_t reserve5;
            uint8_t reserve6;
            uint8_t reserve7;
            uint8_t reserve8;
        }signal;
    } CanSignalVCUCCVS;

    typedef union 
    {
        uint8_t data[8];
        struct 
        {
            uint8_t reserve1;
            uint8_t reserve2;
            uint8_t reserve3;
            uint8_t reserve4;
            uint8_t vehicleErrorLamp:1;
            uint8_t bmsErrorLamp:1;
            uint8_t reserve5_bit3:1;
            uint8_t insulationErrorIndication:1;
            uint8_t reserve5_bit5:1;
            uint8_t driveReadyIndication:1;
            uint8_t socLowIndication:1;
            uint8_t reserve5_bit8:1;
            uint8_t reserve6_bit1:1;
            uint8_t battChargeErrorIndication:1;
            uint8_t reserve6_bit3:1;
            uint8_t tcuErrorIndication:1;
            uint8_t vehicleSTOPIndication:1;
            uint8_t reserve6_bit6:1;
            uint8_t reserve6_bit7:1;
            uint8_t reserve6_bit8:1;
            uint8_t reserve7;
            uint8_t reserve8;
        }signal;
    } CanSignalVCUDispInfo;

    typedef union 
    {
        uint8_t data[8];
        struct 
        {
            uint8_t positionLightStatusIndicator:2;
            uint8_t reserve1_bit3_4:2;
            uint8_t frontFogStatus:2;
            uint8_t reserve1_bit7_8:2;
            uint8_t lowBeamStatus:2;
            uint8_t highBeamStatus:2;
            uint8_t reserve2_bit5_8:4;
            uint8_t reserve3_bit1_2:2;
            uint8_t rearFogStatus:2;
            uint8_t reserve3_bit5_8:4;
            uint8_t reserve4;
            uint8_t reserve5;
            uint8_t reserve6;
            uint8_t reserve7;
            uint8_t reserve8;
        }signal;
    } CanSignalVCUCPD1;

    typedef union 
    {
        uint8_t data[8];
        struct 
        {
            uint8_t reserve1_bit1_2:2;
            uint8_t batteryVoltageLow:6;
            uint8_t batteryVoltageHigh;
            uint8_t reserve3;
            uint8_t reserve4;
            uint8_t reserve5;
            uint8_t leftTurnLightCommandtoIC:2;
            uint8_t rightTurnLightCommandtoIC:2;
            uint8_t reserve6_bit5_8:4;
            uint8_t reserve7;
            uint8_t reserve8;
        }signal;
    } CanSignalVCUCPD3;

    typedef union
    {
        uint8_t data[8];
        struct 
        {
            uint8_t reserve1_bit1_2:2;
            uint8_t driverDoorAjarSwitch:2;
            uint8_t reserve1_bit5_8:4;
            uint8_t passengerDoorAjarSwitch:2;
            uint8_t doorNotClosedInd:2;
            uint8_t reserve2_bit5_8:4;
            uint8_t reserve3;
            uint8_t reserve4;
            uint8_t reserve5;
            uint8_t reserve6;
            uint8_t reserve7;
            uint8_t reserve8;
        }signal;
    }CanSignalVCUConfigStatus;

    typedef union 
    {
       uint8_t data[8];
       struct
       {
            uint8_t reserve1;
            uint8_t reserve2;
            uint8_t reserve3;
            uint8_t reserve4;
            uint8_t reserve5;
            uint8_t reserve6;
            uint8_t reserve7;
            uint8_t EPSErrorLevel;
       }signal;
    }CanSignalEPSError;
    
} // namespace AutoDataService
