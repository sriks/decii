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


################ rsssuite dependency ##################

QT += xmlpatterns
win32:INCLUDEPATH += c:/decii/include
unix:INCLUDEPATH += /home/decii/include
win32:LIBS += -Lc:/decii/lib
unix:LIBS += -L/home/decii/lib
LIBS += -lrssparser
LIBS += -lrssmanager

################ rsssuite dependency ##################
