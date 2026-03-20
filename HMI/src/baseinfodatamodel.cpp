#include "baseinfodatamodel.h"
#include "sm_domain.hpp"
#include "sm_gauge.hpp"
#include <QDebug>

BaseInfoDataModel::BaseInfoDataModel(int updateInterval, QObject *parent)
    : DataModel{updateInterval, parent}
{

}

void BaseInfoDataModel::updateData()
{
    DATA_PROERTY_CHECK(gauge_VCU_MotPwrRate)
    DATA_PROERTY_CHECK(domain_BCM_U_BATT)
    DATA_PROERTY_CHECK(gauge_VCU_DistanceRange)
    DATA_PROERTY_CHECK(gauge_VCU_TotalDistanceRange)
    DATA_PROERTY_CHECK(gauge_SubTotalDistanceRange)
}

DATA_PROPERTY_SIMPLE_IMPLEMENT(BaseInfoDataModel, gauge_VCU_MotPwrRate, int)

DATA_PROPERTY_IMPLEMENT(BaseInfoDataModel, domain_BCM_U_BATT, QString)
{
    //qDebug()<<"domain_BCM_U_BATT123="<<domain_BCM_U_BATT.getValue();
    QString temp_batt = QString::number(domain_BCM_U_BATT.getValue(), 'f',1);
    return  temp_batt;
}

DATA_PROPERTY_IMPLEMENT(BaseInfoDataModel, gauge_VCU_DistanceRange, int)
{
    int m_DistanceRange = gauge_VCU_DistanceRange.getValue();
    if(m_DistanceRange < 0)
    {
        m_DistanceRange = 0;
    }
    return m_DistanceRange;
}

DATA_PROPERTY_IMPLEMENT(BaseInfoDataModel, gauge_VCU_TotalDistanceRange, int)
{
    int m_TotalDistanceRange = gauge_VCU_TotalDistanceRange.getValue();
    if(m_TotalDistanceRange < 0)
    {
        m_TotalDistanceRange = 0;
    }
    return m_TotalDistanceRange;
}

DATA_PROPERTY_IMPLEMENT(BaseInfoDataModel, gauge_SubTotalDistanceRange , int)
{
    int m_SubTotalDistanceRange = gauge_SubTotalDistanceRange.getValue();
    if(m_SubTotalDistanceRange < 0)
    {
        m_SubTotalDistanceRange = 0;
    }
    return m_SubTotalDistanceRange;
}
