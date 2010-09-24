#-------------------------------------------------
#
# Project created by QtCreator 2010-09-15T14:22:13
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = unittest_rssmanager
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    test_rssmanager.cpp

HEADERS += \
    test_rssmanager.h

# rsssuite dependency start
# Any client willing to use rsssuite can copy the following lines in .pro
#####################################################
QT += xmlpatterns
win32: {
    INCLUDEPATH += c:/decii/include
    LIBS += -Lc:/decii/lib }
unix: { # unix support is experimental
    INCLUDEPATH += /home/decii/include
    LIBS += -L/usr/lib }

    LIBS += -lrssparser
    LIBS += -lrssmanager
#####################################################
