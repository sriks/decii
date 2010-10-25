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

include(../embedded-widgets-1.1.0/src/common/common.pri)
include(../embedded-widgets-1.1.0/src/scrollwheel/scrollwheel.pri)
include(../embedded-widgets-1.1.0/src/svgbutton/svgbutton.pri)
RESOURCES += \
            ../embedded-widgets-1.1.0/skins/berylsquare_svgbutton.qrc \
            ../embedded-widgets-1.1.0/skins/beryl_scrollwheel.qrc \
