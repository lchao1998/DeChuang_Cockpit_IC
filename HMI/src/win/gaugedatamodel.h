#ifndef GAUGEDATAMODEL_H
#define GAUGEDATAMODEL_H

#include "datamodel.h"

class GaugeDataModel : public DataModel
{
    Q_OBJECT
public:
    explicit GaugeDataModel(int updateInterval = 50, QObject *parent = nullptr);

protected:
    void updateData() override;

    DATA_PROPERTY_DEFINE(gauge_VCU_VehSpd, int)
    DATA_PROPERTY_DEFINE(gauge_VCU_VehSOC, int)
    DATA_PROPERTY_DEFINE(gauge_VCU_VehShiftLevel, QString)

signals:
        CHANGED_SIGNAL_DEFINE(gauge_VCU_VehSpd)
        CHANGED_SIGNAL_DEFINE(gauge_VCU_VehSOC)
        CHANGED_SIGNAL_DEFINE(gauge_VCU_VehShiftLevel)

private:
      int m_speed;
      int m_soc;
      int m_shiftlevel;
      int m_count;
      int m_soc_count;


};

#endif // GAUGEDATAMODEL_H
