#include "gaugedatamodel.h"
#include "sm_gauge.hpp"
#include <QMap>
#include <QDebug>

#define TABLE_MAX 9

QMap<int, QString> gear_table = {
    {0x7D, "N"},
    {0xDF,"R"},
    {0x7E,"D1"},
    {0x7F,"D2"},
    {0x80,"D3"},
    {0x81,"D4"},
    {0x82,"D5"},
    {0x83,"D6"},
    {0xFA,"X"},
    {0XFC,"X"},
    {0XFE,"X"},
    {0XFF,"X"}
};


GaugeDataModel::GaugeDataModel(int updateInterval, QObject *parent)
    : DataModel{updateInterval, parent}
{
}

void GaugeDataModel::updateData()
{
    DATA_PROERTY_CHECK(gauge_VCU_VehSpd)
    DATA_PROERTY_CHECK(gauge_VCU_VehSOC)
    DATA_PROERTY_CHECK(gauge_VCU_VehShiftLevel)
}

DATA_PROPERTY_IMPLEMENT(GaugeDataModel, gauge_VCU_VehSpd, int)
{
    int speed = gauge_VCU_VehSpd.getValue();
    if(speed > 140)
    {
        speed = 140;
    }

    //qDebug() << "C++: --------Speed=" << speed;
    return speed;
}

DATA_PROPERTY_IMPLEMENT(GaugeDataModel, gauge_VCU_VehSOC, int)
{
    int soc = gauge_VCU_VehSOC.getValue();
    if(soc > 100)
    {
        soc = 100;
    }
    return soc;
}

DATA_PROPERTY_IMPLEMENT(GaugeDataModel, gauge_VCU_VehShiftLevel, QString)
{
    if (!gear_table.contains(gauge_VCU_VehShiftLevel.getValue()))
    {
        return QString("X");
    }
    return gear_table[gauge_VCU_VehShiftLevel.getValue()];
}
