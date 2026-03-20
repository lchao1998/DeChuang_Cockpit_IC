#include "win/gaugedatamodel.h"
#include <QDebug>

#define TABLE_MAX 9

const char* gear_table[TABLE_MAX] = {
    "N",
    "R",
    "D1",
    "D2",
    "D3",
    "D4",
    "D5",
    "D6",
    "P"
};


GaugeDataModel::GaugeDataModel(int updateInterval, QObject *parent)
    : DataModel{updateInterval, parent},
      m_speed(0),
      m_soc(100),
      m_shiftlevel(0),
      m_count(0),
      m_soc_count(0)
{
}

void GaugeDataModel::updateData()
{
    if(m_speed < 140)
    {
        m_speed += 1;
    }
    else
    {
        m_speed = 0;
    }
    if(m_soc_count > 4)
    {
        m_soc_count = 0;
        if(m_soc > 0)
        {
            m_soc -= 1;
        }
        else
        {
            m_soc = 100;
        }
    }
    m_soc_count ++;

    if(m_count > 40)
    {
        m_count = 0;
        if(m_shiftlevel < 8)
        {
            m_shiftlevel += 1;
        }
        else
        {
            m_shiftlevel = 0;
        }
        emit gauge_VCU_VehShiftLevelChanged();
    }
    m_count++;

    emit gauge_VCU_VehSpdChanged();
    emit gauge_VCU_VehSOCChanged();
}

DATA_PROPERTY_IMPLEMENT(GaugeDataModel, gauge_VCU_VehSpd, int)
{
    return m_speed;
}

DATA_PROPERTY_IMPLEMENT(GaugeDataModel, gauge_VCU_VehSOC, int)
{
    return m_soc;
}

DATA_PROPERTY_IMPLEMENT(GaugeDataModel, gauge_VCU_VehShiftLevel, QString)
{
    return QString(gear_table[m_shiftlevel]);
}

