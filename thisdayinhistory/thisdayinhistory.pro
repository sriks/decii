#-------------------------------------------------
#
# Project created by QtCreator 2010-10-06T12:28:38
#
#-------------------------------------------------

#DEFINES += SCREENSAVER
QT       += core gui webkit network xmlpatterns
TARGET = thisdayinhistory
TEMPLATE = app
SOURCES += main.cpp\
        tdih.cpp \
    historyengine.cpp

HEADERS  += tdih.h \
    historyengine.h

# Decii dependencies
win32:{
include(../dgraphicswidget/dgraphicswidget.pri)
include(../rsssuite/rssmanager/rssmanager.pri)
}
