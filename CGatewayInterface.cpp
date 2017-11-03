#include "CGatewayInterface.h"

#include <QDataStream>
#include <QDebug>

#define GET_FROM_SHARED 1

CGatewayInterface::CGatewayInterface()
{
    //Initialisation mémoire partagée
    m_sharedMemory =  new QSharedMemory("rpmsgValues");
    m_sharedMemory->attach(QSharedMemory::ReadOnly);

    //Initialisation timer
    m_timer = new QTimer;
    connect(m_timer,
            SIGNAL(timeout()),
            this,
            SLOT(readSharedMemory()));
    m_timer->start(1000);
}

CGatewayInterface::~CGatewayInterface()
{
    m_timer->stop();
    delete m_timer;

    delete m_sharedMemory;
}

void CGatewayInterface::setLed(bool value)
{
    QMutexLocker lock(&m_mutex);
    QDBusInterface iface(SERVICE_NAME, PROPERTY_PATH, "", QDBusConnection::systemBus());
    if (iface.isValid())
    {
        iface.call("setLed", value);
    }
}

quint32 CGatewayInterface::getCO2()
{
    QMutexLocker lock(&m_mutex);
    QDBusInterface iface(SERVICE_NAME, PROPERTY_PATH, "", QDBusConnection::systemBus());
    if (iface.isValid())
    {
        QDBusReply<quint32> reply = iface.call("getCO2");

        if (reply.isValid())
        {
            return reply.value();
        }
    }
    return 0;
}

quint32 CGatewayInterface::getTVOC()
{
    QMutexLocker lock(&m_mutex);
    QDBusInterface iface(SERVICE_NAME, PROPERTY_PATH, "", QDBusConnection::systemBus());
    if (iface.isValid())
    {
        QDBusReply<quint32> reply = iface.call("getTVOC");

        if (reply.isValid())
        {
            return reply.value();
        }
    }
    return 0;
}

quint32 CGatewayInterface::getStatus()
{
    QMutexLocker lock(&m_mutex);
    QDBusInterface iface(SERVICE_NAME, PROPERTY_PATH, "", QDBusConnection::systemBus());
    if (iface.isValid())
    {
        QDBusReply<quint32> reply = iface.call("getStatus");

        if (reply.isValid())
        {
            return reply.value();
        }
    }
    return 0;
}

void CGatewayInterface::readSharedMemory()
{
    if(GET_FROM_SHARED)
    {
        if (m_sharedMemory->isAttached())
        {
            QByteArray data;
            quint16 co2 = 0;
            quint16 tvoc = 0;
            quint8 status = 0;

            m_sharedMemory->lock();
            data.setRawData((char*)m_sharedMemory->constData(), m_sharedMemory->size());

            QDataStream stream(data);
            stream >> co2
                    >> tvoc
                    >> status;
            m_sharedMemory->unlock();

            qDebug() << co2 << tvoc << status;
        }
        else
            m_sharedMemory->attach();
    }
    else //get from DBUS
    {
        qDebug() << getCO2() << getTVOC() << getStatus();
    }
}

