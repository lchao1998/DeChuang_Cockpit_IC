#include "win/baseinfodatamodel.h"

BaseInfoDataModel::BaseInfoDataModel(int updateInterval, QObject *parent)
    : DataModel{updateInterval, parent},
      m_pwrrate(0),
      m_batt(6.0),
      m_DistanceRange(999),
      m_TotalDistanceRange(0),
      m_SubTotalDistanceRange(0)
{

}

void BaseInfoDataModel::updateData()
{
    if(m_pwrrate > -60 && m_pwrrate <= 0)
    {
        m_pwrrate--;
    }
    else if(m_pwrrate >= 0 && m_pwrrate < 100)
    {
        m_pwrrate ++;
    }
    else
    {
       m_pwrrate = 1;
    }

    if(m_batt < 37.0)
    {
        m_batt +=0.1;
    }
    else
    {
        m_batt = 6.0;
    }
    if(m_DistanceRange > 0)
    {
        m_DistanceRange--;
        m_TotalDistanceRange++;
        m_SubTotalDistanceRange++;
    }
    else
    {
        m_DistanceRange = 999;
        m_TotalDistanceRange = 0;
        m_SubTotalDistanceRange = 0;
    }
    emit gauge_VCU_MotPwrRateChanged();
    emit domain_BCM_U_BATTChanged();
    emit gauge_VCU_DistanceRangeChanged();
    emit gauge_VCU_TotalDistanceRangeChanged();
    emit gauge_SubTotalDistanceRangeChanged();
}

DATA_PROPERTY_IMPLEMENT(BaseInfoDataModel, gauge_VCU_MotPwrRate, int)
{
    return m_pwrrate;
}

DATA_PROPERTY_IMPLEMENT(BaseInfoDataModel, domain_BCM_U_BATT, QString)
{
    QString temp_batt = QString::number(m_batt, 'f',1);//设置精度
    return  temp_batt;
}

DATA_PROPERTY_IMPLEMENT(BaseInfoDataModel, gauge_VCU_DistanceRange, int)
{
    return m_DistanceRange;
}

DATA_PROPERTY_IMPLEMENT(BaseInfoDataModel, gauge_VCU_TotalDistanceRange, int)
{
    return m_TotalDistanceRange;
}

DATA_PROPERTY_IMPLEMENT(BaseInfoDataModel, gauge_SubTotalDistanceRange, int)
{
    return m_SubTotalDistanceRange;
}
