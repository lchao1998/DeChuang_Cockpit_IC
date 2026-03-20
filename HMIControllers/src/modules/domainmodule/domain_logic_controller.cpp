#include "domain_logic_controller.hpp"
#include "sm_domain.hpp"

DomainController::DomainController()
    : m_domainClient(AutoSOCM::getApplication()->createClient(IPC_SERVICE_ID, IPC_SERVICE_INSTANCE_DOMAININFO)),
      m_domainClientAvailability(false)
{
    CHECKCLIENTAVAILABILITY(domain);
    m_domainClient->setPollable(true);
}

DomainController::~DomainController()
{

}

void DomainController::onProcess()
{
    bool ret = false; 
    ret = m_domainClient->poll();
    // if (ret != true)
    // {
    //     return ;
    // }

     if (m_domainClientAvailability) {
        int tmp_VCU_VehErrInd = m_domainClient->getAttribute("VCU_VehErrInd").toInt();
        domain_VCU_VehErrInd.setValue(tmp_VCU_VehErrInd);

        int tmp_VCU_BMSErrInd = m_domainClient->getAttribute("VCU_BMSErrInd").toInt();
        domain_VCU_BMSErrInd.setValue(tmp_VCU_BMSErrInd);

        int tmp_VCU_InsErrInd = m_domainClient->getAttribute("VCU_InsErrInd").toInt();
        domain_VCU_InsErrInd.setValue(tmp_VCU_InsErrInd);

        int tmp_VCU_DrvReadyInd = m_domainClient->getAttribute("VCU_DrvReadyInd").toInt();
        domain_VCU_DrvReadyInd.setValue(tmp_VCU_DrvReadyInd);

        int tmp_VCU_SOCLowInd  = m_domainClient->getAttribute("VCU_SOCLowInd ").toInt();
        domain_VCU_SOCLowInd .setValue(tmp_VCU_SOCLowInd );

        int tmp_VCU_BattChrgErrInd = m_domainClient->getAttribute("VCU_BattChrgErrInd").toInt();
        domain_VCU_BattChrgErrInd.setValue(tmp_VCU_BattChrgErrInd);

        int tmp_VCU_TCUErrInd = m_domainClient->getAttribute("VCU_TCUErrInd").toInt();
        domain_VCU_TCUErrInd.setValue(tmp_VCU_TCUErrInd);

        int tmp_VCU_VehSTOPInd = m_domainClient->getAttribute("VCU_VehSTOPInd").toInt();
        domain_VCU_VehSTOPInd.setValue(tmp_VCU_VehSTOPInd);

        int tmp_BCM_PosLgtStInd = m_domainClient->getAttribute("BCM_PosLgtStInd").toInt();
        domain_BCM_PosLgtStInd.setValue(tmp_BCM_PosLgtStInd);

        int tmp_BCM_FrontFogLightSts = m_domainClient->getAttribute("BCM_FrontFogLightSts").toInt();
        domain_BCM_FrontFogLightSts.setValue(tmp_BCM_FrontFogLightSts);

        int tmp_BCM_LBeamSt = m_domainClient->getAttribute("BCM_LBeamSt").toInt();
        domain_BCM_LBeamSt.setValue(tmp_BCM_LBeamSt);

        int tmp_BCM_HBeamSt = m_domainClient->getAttribute("BCM_HBeamSt").toInt();
        domain_BCM_HBeamSt.setValue(tmp_BCM_HBeamSt);

        int tmp_BCM_RFogLgtSt = m_domainClient->getAttribute("BCM_RFogLgtSt").toInt();
        domain_BCM_RFogLgtSt.setValue(tmp_BCM_RFogLgtSt);

        float tmp_BCM_U_BATT = m_domainClient->getAttribute("BCM_U_BATT").toFloat();
        domain_BCM_U_BATT.setValue(tmp_BCM_U_BATT);

        int tmp_BCM_L_TurnLgtCmdToIC = m_domainClient->getAttribute("BCM_L_TurnLgtCmdToIC").toInt();
        domain_BCM_L_TurnLgtCmdToIC.setValue(tmp_BCM_L_TurnLgtCmdToIC);

        int tmp_BCM_R_TurnLgtCmdToIC = m_domainClient->getAttribute("BCM_R_TurnLgtCmdToIC").toInt();
        domain_BCM_R_TurnLgtCmdToIC.setValue(tmp_BCM_R_TurnLgtCmdToIC);

        int tmp_DCM_DriverDoorAjarSW = m_domainClient->getAttribute("BCM_DCM_DriverDoorAjarSW").toInt();
        domain_DCM_DriverDoorAjarSW.setValue(tmp_DCM_DriverDoorAjarSW);

        int tmp_DCM_PassengerDoorAjarSW = m_domainClient->getAttribute("DCM_PassengerDoorAjarSW").toInt();
        domain_DCM_PassengerDoorAjarSW.setValue(tmp_DCM_PassengerDoorAjarSW);

        int tmp_DCM_DoorNotClosedInd = m_domainClient->getAttribute("DCM_DoorNotClosedInd").toInt();
        domain_DCM_DoorNotClosedInd.setValue(tmp_DCM_DoorNotClosedInd);

        int tmp_EPS_ErrLevel = m_domainClient->getAttribute("EPS_ErrLevel").toInt();
        domain_EPS_ErrLevel.setValue(tmp_EPS_ErrLevel);
        //logDebug("[AutoLogicController] EPS_ErrLevel: %i \n", tmp_EPS_ErrLevel);
     }
}

