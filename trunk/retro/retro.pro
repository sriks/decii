#-------------------------------------------------
#
# Project created by QtCreator 2010-09-21T18:45:25
#
#-------------------------------------------------

QT       += core gui network webkit

TARGET = retro
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    feedview.cpp

HEADERS  += mainwindow.h \
    feedview.h

FORMS    += mainwindow.ui


# rsssuite dependency start
# Any client willing to use rsssuite can copy the following lines in .pro
QT += xmlpatterns
include(../rsssuite/rssmanager/rssmanager.pri)
include(../DGraphicsWidget/dgraphicswidget.pri)
include(../KSegmentView/ksegmentview.pri)
