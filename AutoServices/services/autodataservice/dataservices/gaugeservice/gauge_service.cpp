#include "gauge_service.hpp"
#include "service_define.hpp"

namespace AutoDataService
{
    GaugeService::GaugeService(ServiceInterface* manager) :
    ServiceBase(manager, IPC_SERVICE_ID, IPC_SERVICE_INSTANCE_GAUGEINFO, IPC_Service_Name_Gauge)
    {
        init();
    }
    
    GaugeService::~GaugeService()
    {

    }

    void GaugeService::init()
    {
        m_Service->initAttribute("VCU_VehVolt", 0);
		m_Service->initAttribute("VCU_VehSOC", 0);
		m_Service->initAttribute("VCU_DistanceRange", 0);
		m_Service->initAttribute("VCU_VehSpd", 0);
		m_Service->initAttribute("VCU_VehShiftLevel", 0);
		m_Service->initAttribute("VCU_MotPwrRate", 0);
		m_Service->initAttribute("VCU_ParkBrkSW", 0);
        m_Service->initAttribute("VCU_CCActive", 0);
        m_Service->initAttribute("VCU_CCEnableSW", 0);
    }
} 