#-------------------------------------------------
#
# Project created by QtCreator 2010-11-13T21:38:50
#
#-------------------------------------------------

QT       += core gui network xmlpatterns webkit

TARGET = ranloc
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    locationengine.cpp

HEADERS  += mainwindow.h \
    locationengine.h

FORMS    += mainwindow.ui

#include(../custombutton/custombutton.pri)

#CONFIG += mobility
#MOBILITY =

symbian{
LIBS += -lesock
TARGET.CAPABILITY = NetworkServices

TARGET.UID3 = 0xec6ff120
# TARGET.CAPABILITY +=
TARGET.EPOCSTACKSIZE = 0x14000
TARGET.EPOCHEAPSIZE = 0x020000 0x800000
}

