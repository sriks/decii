#-------------------------------------------------
#
# Project created by QtCreator 2010-09-21T18:45:25
#
#-------------------------------------------------

QT       += core gui

TARGET = retro
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui


# rsssuite dependency start
# Any client willing to use rsssuite can copy the following lines in .pro
QT += xmlpatterns
include(../rsssuite/rssmanager/rssmanager.pri)
