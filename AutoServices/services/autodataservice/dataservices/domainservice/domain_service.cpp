#include "domain_service.hpp"
#include "service_define.hpp"

namespace AutoDataService
{

	DomainService::DomainService(ServiceInterface* manager) :
    ServiceBase(manager, IPC_SERVICE_ID, IPC_SERVICE_INSTANCE_DOMAININFO, IPC_Service_Name_Domain)
    {
        init();
    }
    
	DomainService::~DomainService()
    {

    }

    void DomainService::init()
    {
		m_Service->initAttribute("VCU_VehErrInd", 0);
		m_Service->initAttribute("VCU_BMSErrInd", 0);
		m_Service->initAttribute("VCU_InsErrInd", 0);
		m_Service->initAttribute("VCU_DrvReadyInd", 0);
		m_Service->initAttribute("VCU_SOCLowInd ", 0);
		m_Service->initAttribute("VCU_BattChrgErrInd", 0);
		m_Service->initAttribute("VCU_TCUErrInd", 0);
		m_Service->initAttribute("VCU_VehSTOPInd", 0);

		m_Service->initAttribute("BCM_PosLgtStInd", 0);
		m_Service->initAttribute("BCM_FrontFogLightSts", 0);
		m_Service->initAttribute("BCM_LBeamSt", 0);
		m_Service->initAttribute("BCM_HBeamSt", 0);
		m_Service->initAttribute("BCM_RFogLgtSt", 0);

		m_Service->initAttribute("BCM_U_BATT", 0);
		m_Service->initAttribute("BCM_L_TurnLgtCmdToIC", 0);
		m_Service->initAttribute("BCM_R_TurnLgtCmdToIC", 0);
		
		m_Service->initAttribute("DCM_DriverDoorAjarSW", 0);
		m_Service->initAttribute("DCM_PassengerDoorAjarSW", 0);
		m_Service->initAttribute("DCM_DoorNotClosedInd", 0);

		m_Service->initAttribute("EPS_ErrLevel", 0);

    }
} 