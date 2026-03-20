#include "sm_gauge.hpp"

namespace SharedMemory
{
    SM_POOL_IMPLEMENTATION(SharedMemoryPool_Gauge);

    SM_NUMBER_IMPLEMENTATION(int, gauge_VCU_VehVolt, SharedMemoryPool_Gauge, 0); //车辆总电压
    SM_NUMBER_IMPLEMENTATION(int, gauge_VCU_VehSOC, SharedMemoryPool_Gauge, 0); //车辆荷电状态
    SM_NUMBER_IMPLEMENTATION(int, gauge_VCU_DistanceRange, SharedMemoryPool_Gauge, 0); //整车续驶里程
    SM_NUMBER_IMPLEMENTATION(int, gauge_VCU_TotalDistanceRange, SharedMemoryPool_Gauge, 0); //总里程
    SM_NUMBER_IMPLEMENTATION(int, gauge_SubTotalDistanceRange, SharedMemoryPool_Gauge, 0); //小计里程
    SM_NUMBER_IMPLEMENTATION(int, gauge_VCU_VehSpd, SharedMemoryPool_Gauge, 0); //车速
    SM_NUMBER_IMPLEMENTATION(int, gauge_VCU_VehShiftLevel, SharedMemoryPool_Gauge, 0); //整车挡位
    SM_NUMBER_IMPLEMENTATION(int, gauge_VCU_MotPwrRate, SharedMemoryPool_Gauge, 0); //电机功率百分比
    SM_NUMBER_IMPLEMENTATION(int, gauge_VCU_ParkBrkSW, SharedMemoryPool_Gauge, 0); //驻车开关
    SM_NUMBER_IMPLEMENTATION(int, gauge_VCU_CCActive, SharedMemoryPool_Gauge, 0); //巡航控制激活
    SM_NUMBER_IMPLEMENTATION(int, gauge_VCU_CCEnableSW, SharedMemoryPool_Gauge, 0); //巡航控制系统启动开关
}

