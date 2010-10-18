#-------------------------------------------------
#
# Project created by QtCreator 2010-10-18T23:01:56
#
#-------------------------------------------------

QT       += core gui

TARGET = cholestrolanalyzer
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    dcadataentrywidget.cpp \
    dgraphicstextwidget.cpp

HEADERS  += widget.h \
    dcadataentrywidget.h \
    dgraphicstextwidget.h



include(/home/srikanth/development/trunk/qtembedded/src/common/common.pri)
include(/home/srikanth/development/trunk/qtembedded/src/scrollwheel/scrollwheel.pri)

RESOURCES += /home/srikanth/development/trunk/qtembedded/skins/beryl_scrollwheel.qrc
