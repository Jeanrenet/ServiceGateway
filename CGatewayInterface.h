#ifndef CGATEWAYINTERFACE_H
#define CGATEWAYINTERFACE_H

#include <QDBusInterface>
#include <QDBusReply>
#include <QMutex>
#include <QTimer>
#include <QSharedMemory>

#define SERVICE_NAME    "iot.service.rpmsg"
#define PROPERTY_PATH   "/"

class CGatewayInterface : public QObject
{
    Q_OBJECT
public:
    CGatewayInterface();
    ~CGatewayInterface();

public:

    //setters
    void setLed(bool value);

    //getters
    quint32 getCO2();
    quint32 getTVOC();
    quint32 getStatus();

protected slots:
    void readSharedMemory();

private:
    QMutex              m_mutex;
    QTimer              *m_timer;
    QSharedMemory       *m_sharedMemory;
};

#endif // CGATEWAYINTERFACE_H
