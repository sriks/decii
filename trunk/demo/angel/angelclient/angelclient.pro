#-------------------------------------------------
#
# Project created by QtCreator 2010-11-28T22:55:36
#
#-------------------------------------------------

QT       += core gui network

TARGET = angelclient
TEMPLATE = app


SOURCES += main.cpp\
        angelclient.cpp

HEADERS  += angelclient.h

FORMS    += angelclient.ui

CONFIG += mobility
MOBILITY = 

symbian {
    TARGET.UID3 = 0xea32c6e1
    TARGET.CAPABILITY += NetworkServices ReadUserData WriteUserData
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x020000 0x800000
}
