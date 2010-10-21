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

include(../qtembedded/src/common/common.pri)
include(../qtembedded/src/scrollwheel/scrollwheel.pri)
include(../qtembedded/src/svgbutton/svgbutton.pri)
RESOURCES += \
            /home/srikanth/development/trunk/qtembedded/skins/berylsquare_svgbutton.qrc \
            ../qtembedded/skins/beryl_scrollwheel.qrc \
