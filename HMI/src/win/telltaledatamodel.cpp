#include "win/telltaledatamodel.h"

#define DATA_PROPERTY_EASY_IMPLEMENT(func, name, type) \
    type func::get##name() const \
{ \
    return m_switch; \
}


TelltaleDataModel::TelltaleDataModel(int updateInterval, QObject *parent)
    : DataModel{updateInterval, parent},
      m_index(0),
      m_switch(false)
{
}

void TelltaleDataModel::updateData()
{
    if(m_index < 12)
    {
        m_switch = true;
    }
    else
    {
        m_switch = false;
    }

    switch(m_index)
    {
    case 0:
        emit domain_BCM_PosLgtStIndChanged();
        break;
    case 1:
        emit domain_BCM_HBeamStChanged();
        break;
    case 2:
        emit domain_BCM_FrontFogLightStsChanged();
        break;
    case 3:
        emit domain_BCM_RFogLgtStChanged();
        break;
    case 4:
        emit domain_BCM_L_TurnLgtCmdToICChanged();
        break;
    case 5:
        emit domain_BCM_R_TurnLgtCmdToICChanged();
        break;
    case 6:
        emit domain_DCM_DoorNotClosedIndChanged();
        break;
    case 7:
        emit domain_VCU_VehErrIndChanged();
        break;
    case 8:
        emit gauge_VCU_ParkBrkSWChanged();
        break;
    case 9:
        emit domain_VCU_BMSErrIndChanged();
        break;
    case 10:
        emit domain_VCU_DrvReadyIndChanged();
        break;
    case 11:
        emit domain_VCU_VehSTOPIndChanged();
        break;
    default:
        emit domain_BCM_PosLgtStIndChanged();
        emit domain_BCM_HBeamStChanged();
        emit domain_BCM_FrontFogLightStsChanged();
        emit domain_BCM_RFogLgtStChanged();
        emit domain_BCM_L_TurnLgtCmdToICChanged();
        emit domain_BCM_R_TurnLgtCmdToICChanged();
        emit domain_DCM_DoorNotClosedIndChanged();
        emit domain_VCU_VehErrIndChanged();
        emit gauge_VCU_ParkBrkSWChanged();
        emit domain_VCU_BMSErrIndChanged();
        emit domain_VCU_DrvReadyIndChanged();
        emit domain_VCU_VehSTOPIndChanged();
        break;
    }

    m_index++;
    if(m_index > 12)
    {
        m_index = 0;
    }
}

DATA_PROPERTY_EASY_IMPLEMENT(TelltaleDataModel, domain_BCM_PosLgtStInd, bool)
DATA_PROPERTY_EASY_IMPLEMENT(TelltaleDataModel, domain_BCM_HBeamSt, bool)
DATA_PROPERTY_EASY_IMPLEMENT(TelltaleDataModel, domain_BCM_FrontFogLightSts, bool)
DATA_PROPERTY_EASY_IMPLEMENT(TelltaleDataModel, domain_BCM_RFogLgtSt, bool)
DATA_PROPERTY_EASY_IMPLEMENT(TelltaleDataModel, domain_BCM_L_TurnLgtCmdToIC, bool)
DATA_PROPERTY_EASY_IMPLEMENT(TelltaleDataModel, domain_BCM_R_TurnLgtCmdToIC, bool)
DATA_PROPERTY_EASY_IMPLEMENT(TelltaleDataModel, domain_DCM_DoorNotClosedInd, bool)
DATA_PROPERTY_EASY_IMPLEMENT(TelltaleDataModel, domain_VCU_VehErrInd, bool)
DATA_PROPERTY_EASY_IMPLEMENT(TelltaleDataModel, gauge_VCU_ParkBrkSW, bool)
DATA_PROPERTY_EASY_IMPLEMENT(TelltaleDataModel, domain_VCU_BMSErrInd, bool)
DATA_PROPERTY_EASY_IMPLEMENT(TelltaleDataModel, domain_VCU_DrvReadyInd, bool)
DATA_PROPERTY_EASY_IMPLEMENT(TelltaleDataModel, domain_VCU_VehSTOPInd, bool)


