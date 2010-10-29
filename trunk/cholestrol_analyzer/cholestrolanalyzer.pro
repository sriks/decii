#-------------------------------------------------
#
# Project created by QtCreator 2010-10-18T23:01:56
#
#-------------------------------------------------

DEFINES  += CHOLESTROL_WIDGET
QT       += core gui

TARGET = cholestrolanalyzer
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    dcadataentrywidget.cpp


HEADERS  += widget.h \
    dcadataentrywidget.h

INCLUDEPATH += ../embedded-widgets-1.1.0/src/scrollwheel/
HEADERS += ../embedded-widgets-1.1.0/src/scrollwheel/qtscrollwheel.h
SOURCES += ../embedded-widgets-1.1.0/src/scrollwheel/qtscrollwheel.cpp

INCLUDEPATH += ../DGraphicsWidget/
HEADERS += ../DGraphicsWidget/dgraphicstextwidget.h
SOURCES += ../DGraphicsWidget/dgraphicstextwidget.cpp

include(../embedded-widgets-1.1.0/src/common/common.pri)
RESOURCES += \
            ../embedded-widgets-1.1.0/skins/berylsquare_svgbutton.qrc \
            ../embedded-widgets-1.1.0/skins/beryl_scrollwheel.qrc \

