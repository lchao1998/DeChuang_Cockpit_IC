#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QThread>
#include <QTimer>

#define DATA_PROPERTY_DEFINE(name, type) \
Q_PROPERTY(type name READ get##name NOTIFY name##Changed) \
public: \
    type get##name() const;

#define DATA_PROPERTY_IMPLEMENT(func, name, type) \
type func::get##name() const

#define DATA_PROPERTY_SIMPLE_IMPLEMENT(func, name, type) \
type func::get##name() const \
{ \
    return name.getValue(); \
}

#define CHANGED_SIGNAL_DEFINE(name) \
    void name##Changed();

#define DATA_PROERTY_CHECK(name) \
    if (name.isModified()) {  \
        emit name##Changed();  \
    }

class DataModel : public QThread
{
    Q_OBJECT
public:
    explicit DataModel(int updateInterval = 2, QObject *parent = nullptr)
        : QThread{parent},
          m_updateInterval(updateInterval)
    {
        start();
    }

protected:
    virtual void updateData() = 0;
    void run() override
    {
            QTimer timer;
            connect(&timer, &QTimer::timeout, this, &DataModel::updateData);
            timer.start(m_updateInterval);
            exec();
    }

    int m_updateInterval;
};

#endif // DATAMODEL_H
