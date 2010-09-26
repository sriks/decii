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
#####################################################
# For unix systems, dlls and headers are copied to standard /usr/lib and /usr/include respectively
# hence they need not be mentioned in unix targets.

QT += xmlpatterns
win32: {
    INCLUDEPATH += c:/decii/include
    LIBS += -Lc:/decii/lib
        }
    LIBS += -lrssparser
    LIBS += -lrssmanager
#####################################################


