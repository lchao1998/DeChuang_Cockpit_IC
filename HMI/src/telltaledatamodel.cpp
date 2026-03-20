#include "telltaledatamodel.h"
#include "sm_domain.hpp"
#include "sm_gauge.hpp"
#include <QDebug>

#define DATA_PROPERTY_EASY_IMPLEMENT(func, name, type) \
    type func::get##name() const \
{ \
    return name.getValue() == 1; \
} \


TelltaleDataModel::TelltaleDataModel(int updateInterval, QObject *parent)
    : DataModel{updateInterval, parent}
{
}

void TelltaleDataModel::updateData()
{
    DATA_PROERTY_CHECK(domain_BCM_PosLgtStInd)
    DATA_PROERTY_CHECK(domain_BCM_HBeamSt)
    DATA_PROERTY_CHECK(domain_BCM_FrontFogLightSts)
    DATA_PROERTY_CHECK(domain_BCM_RFogLgtSt)
    DATA_PROERTY_CHECK(domain_BCM_L_TurnLgtCmdToIC)
    DATA_PROERTY_CHECK(domain_BCM_R_TurnLgtCmdToIC)
    DATA_PROERTY_CHECK(domain_DCM_DoorNotClosedInd)
    DATA_PROERTY_CHECK(domain_VCU_VehErrInd)
    DATA_PROERTY_CHECK(gauge_VCU_ParkBrkSW)
    DATA_PROERTY_CHECK(domain_VCU_BMSErrInd)
    DATA_PROERTY_CHECK(domain_VCU_DrvReadyInd)
    DATA_PROERTY_CHECK(domain_VCU_VehSTOPInd)

    //qDebug() << "C++: ------------domain_BCM_L_TurnLgtCmdToIC----------=" << domain_BCM_L_TurnLgtCmdToIC.getValue();
    //qDebug() << "C++: ------------domain_BCM_R_TurnLgtCmdToIC----------=" << domain_BCM_R_TurnLgtCmdToIC.getValue();

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

