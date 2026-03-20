#include "gauge_logic_controller.hpp"
#include "log/log.hpp"
#include <fstream>
#include <string>
#include <chrono>
#include <unistd.h>

GaugeLogicController::GaugeLogicController()
    : Module(),
    m_GaugeInfo(),
    m_GaugeClient(AutoSOCM::getApplication()->createClient(IPC_SERVICE_ID, IPC_SERVICE_INSTANCE_GAUGEINFO)),
    m_GaugeClientAvailability(false),
    m_VCU_TotalDistanceRange(0),
    m_advancedSpanRange(0),
    m_firstDistanceRange(-1),
    m_saveInterval(0)
{
    memset(&m_GaugeInfo, 0, sizeof(SM_Gauge));
    CHECKCLIENTAVAILABILITY(Gauge);

    m_GaugeClient->setPollable(true);

    std::ifstream configFile;
    configFile.open("config-liuzuyang.txt");

    if (!configFile.is_open())
    {
        logError("[AutoLogicController] config file open failed!!!");
        return;
    }
    int tmp_VCU_TotalDistanceRangeRead = 0;

    configFile >> tmp_VCU_TotalDistanceRangeRead;

    m_VCU_TotalDistanceRange = tmp_VCU_TotalDistanceRangeRead;

    //("[AutoLogicController] m_VCU_TotalDistanceRange: --------------------------------%i", m_VCU_TotalDistanceRange);
    configFile.close();
}

GaugeLogicController::~GaugeLogicController()
{

}

void GaugeLogicController::onProcess()
{
    bool ret = false; 
    ret = m_GaugeClient->poll();
    // if (ret != true)
    // {
    //     return ;
    // }

    static std::chrono::steady_clock::time_point sta_beforeTime;

    if (m_GaugeClientAvailability) {

        int tmp_VCU_VehVolt = m_GaugeClient->getAttribute("VCU_VehVolt").toUInt();
        gauge_VCU_VehVolt.setValue(tmp_VCU_VehVolt);
        //logInfo("[AutoLogicController] tmp_VCU_VehVolt: %i", tmp_VCU_VehVolt);

        int tmp_VCU_VehSOC = m_GaugeClient->getAttribute("VCU_VehSOC").toUInt();
        gauge_VCU_VehSOC.setValue(tmp_VCU_VehSOC);
        //logInfo("[AutoLogicController] tmp_VCU_VehVolt: %i", tmp_VCU_VehVolt);

        int tmp_VCU_DistanceRange = m_GaugeClient->getAttribute("VCU_DistanceRange").toUInt(); 
        gauge_VCU_DistanceRange.setValue(tmp_VCU_DistanceRange);
        
        //logInfo("[AutoLogicController] tmp_VCU_DistanceRange: %i", tmp_VCU_DistanceRange);

        int tmp_VCU_VehSpd = m_GaugeClient->getAttribute("VCU_VehSpd").toUInt();
        gauge_VCU_VehSpd.setValue(tmp_VCU_VehSpd);
        //logDebug("[AutoLogicController] tmp_VCU_VehSpd: %i", tmp_VCU_VehSpd);
        double duration_millsecond = 0;
        if (sta_beforeTime == std::chrono::steady_clock::time_point()) {
            sta_beforeTime = std::chrono::steady_clock::now();
        } else {
            auto afterTime = std::chrono::steady_clock::now();
            //获取毫秒级时间间隔
            duration_millsecond = std::chrono::duration<double, std::milli>(afterTime - sta_beforeTime).count();
            m_saveInterval += duration_millsecond;
            sta_beforeTime = afterTime;
        }

        double advancedSpan = (double)tmp_VCU_VehSpd * (duration_millsecond / (60 * 60 * 1000));
        m_advancedSpanRange += advancedSpan;
        m_VCU_TotalDistanceRange += advancedSpan;
        gauge_VCU_TotalDistanceRange.setValue(m_VCU_TotalDistanceRange);
        gauge_SubTotalDistanceRange.setValue(m_advancedSpanRange);

        if (m_saveInterval >= 1000) {
            std::ofstream configFile("config-liuzuyang.txt");
            int tmp_VCU_TotalDistanceRangeWrite = m_VCU_TotalDistanceRange;
            if (configFile.is_open()) {
                //configFile << std::to_string(tmp_VCU_TotalDistanceRange) << std::endl;
                configFile << tmp_VCU_TotalDistanceRangeWrite;

                configFile.flush();
                sync();
                configFile.close();
            }

            //("[AutoLogicController] m_VCU_TotalDistanceRange: %f", m_VCU_TotalDistanceRange);
            m_saveInterval = 0;
        }

        //logDebug("[AutoLogicController] tmp_VCU_VehSpd: %i m_advancedSpanRange: %f  m_VCU_TotalDistanceRange: %f", 
        //    tmp_VCU_VehSpd, m_advancedSpanRange, m_VCU_TotalDistanceRange);
        

        int tmp_VCU_VehShiftLevel = m_GaugeClient->getAttribute("VCU_VehShiftLevel").toUInt();
        gauge_VCU_VehShiftLevel.setValue(tmp_VCU_VehShiftLevel);
        //logInfo("[AutoLogicController] tmp_VCU_VehShiftLevel: %i", tmp_VCU_VehShiftLevel);
        int tmp_VCU_MotPwrRate = m_GaugeClient->getAttribute("VCU_MotPwrRate").toUInt();
        gauge_VCU_MotPwrRate.setValue(tmp_VCU_MotPwrRate);

        int tmp_VCU_ParkBrkSW = m_GaugeClient->getAttribute("VCU_ParkBrkSW").toUInt();
        gauge_VCU_ParkBrkSW.setValue(tmp_VCU_ParkBrkSW);
        //logInfo("[AutoLogicController] tmp_VCU_ParkBrkSW: %i", tmp_VCU_ParkBrkSW);

        int tmp_VCU_CCActive = m_GaugeClient->getAttribute("VCU_CCActive").toUInt();
        gauge_VCU_CCActive.setValue(tmp_VCU_CCActive);
        //logDebug("[AutoLogicController] VCU_CCActive: %i \n", tmp_VCU_CCActive);

        int tmp_VCU_CCEnableSW = m_GaugeClient->getAttribute("VCU_CCEnableSW").toUInt();
        gauge_VCU_CCEnableSW.setValue(tmp_VCU_CCEnableSW);
        //logDebug("[AutoLogicController] VCU_CCEnableSW: %i \n", tmp_VCU_CCEnableSW);
    }
}


