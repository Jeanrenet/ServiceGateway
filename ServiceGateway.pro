QT += core dbus
QT -= gui

TARGET = ServiceGateway
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    CGatewayInterface.cpp

HEADERS += \
    CGatewayInterface.h

target.path= /usr/bin

INSTALLS += target

