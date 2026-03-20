#ifndef TELLTALEDATAMODEL_H
#define TELLTALEDATAMODEL_H

#include "datamodel.h"

class TelltaleDataModel : public DataModel
{
    Q_OBJECT
public:
    explicit TelltaleDataModel(int updateInterval = 100, QObject *parent = nullptr);

protected:
    void updateData() override;

    DATA_PROPERTY_DEFINE(domain_BCM_PosLgtStInd,bool)//位置灯
    DATA_PROPERTY_DEFINE(domain_BCM_HBeamSt,bool)//远光灯
    DATA_PROPERTY_DEFINE(domain_BCM_FrontFogLightSts,bool)//前雾灯
    DATA_PROPERTY_DEFINE(domain_BCM_RFogLgtSt,bool)//后雾灯
    DATA_PROPERTY_DEFINE(domain_BCM_L_TurnLgtCmdToIC,bool)//左转向灯
    DATA_PROPERTY_DEFINE(domain_BCM_R_TurnLgtCmdToIC,bool)//右转向灯
    DATA_PROPERTY_DEFINE(domain_DCM_DoorNotClosedInd,bool)//车门未关报警
    DATA_PROPERTY_DEFINE(domain_VCU_VehErrInd,bool)//整车故障指示灯
    DATA_PROPERTY_DEFINE(gauge_VCU_ParkBrkSW,bool)//驻车制动指示灯
    DATA_PROPERTY_DEFINE(domain_VCU_BMSErrInd,bool)//动力电池故障指示灯
    DATA_PROPERTY_DEFINE(domain_VCU_DrvReadyInd,bool)//驱动就绪指示灯
    DATA_PROPERTY_DEFINE(domain_VCU_VehSTOPInd,bool)//重大故障指示灯



signals:
        CHANGED_SIGNAL_DEFINE(domain_BCM_PosLgtStInd)
        CHANGED_SIGNAL_DEFINE(domain_BCM_HBeamSt)
        CHANGED_SIGNAL_DEFINE(domain_BCM_FrontFogLightSts)
        CHANGED_SIGNAL_DEFINE(domain_BCM_RFogLgtSt)
        CHANGED_SIGNAL_DEFINE(domain_BCM_L_TurnLgtCmdToIC)
        CHANGED_SIGNAL_DEFINE(domain_BCM_R_TurnLgtCmdToIC)
        CHANGED_SIGNAL_DEFINE(domain_DCM_DoorNotClosedInd)
        CHANGED_SIGNAL_DEFINE(domain_VCU_VehErrInd)
        CHANGED_SIGNAL_DEFINE(gauge_VCU_ParkBrkSW)
        CHANGED_SIGNAL_DEFINE(domain_VCU_BMSErrInd)
        CHANGED_SIGNAL_DEFINE(domain_VCU_DrvReadyInd)
        CHANGED_SIGNAL_DEFINE(domain_VCU_VehSTOPInd)

private:


};

#endif // TELLTALEDATAMODEL_H
