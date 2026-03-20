#ifndef BASEINFODATAMODEL_H
#define BASEINFODATAMODEL_H

#include "datamodel.h"

class BaseInfoDataModel : public DataModel
{
    Q_OBJECT
public:
    explicit BaseInfoDataModel(int updateInterval = 100, QObject *parent = nullptr);

protected:
    void updateData() override;

    DATA_PROPERTY_DEFINE(gauge_VCU_MotPwrRate, int)//电机功率
    DATA_PROPERTY_DEFINE(domain_BCM_U_BATT, QString)//蓄电池电压
    DATA_PROPERTY_DEFINE(gauge_VCU_DistanceRange, int)//续驶里程
    DATA_PROPERTY_DEFINE(gauge_VCU_TotalDistanceRange, int)//总里程
    DATA_PROPERTY_DEFINE(gauge_SubTotalDistanceRange, int)//小计里程

signals:
        CHANGED_SIGNAL_DEFINE(gauge_VCU_MotPwrRate)
        CHANGED_SIGNAL_DEFINE(domain_BCM_U_BATT)
        CHANGED_SIGNAL_DEFINE(gauge_VCU_DistanceRange)
        CHANGED_SIGNAL_DEFINE(gauge_VCU_TotalDistanceRange)
        CHANGED_SIGNAL_DEFINE(gauge_SubTotalDistanceRange)

private:
};

#endif // BASEINFODATAMODEL_H
