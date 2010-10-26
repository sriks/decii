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
    dcadataentrywidget.cpp \
    dgraphicstextwidget.cpp \
    #dgraphicsproxywidget.cpp

HEADERS  += widget.h \
    dcadataentrywidget.h \
    dgraphicstextwidget.h \
   # dgraphicsproxywidget.h

INCLUDEPATH += ../embedded-widgets-1.1.0/src/scrollwheel/
HEADERS += ../embedded-widgets-1.1.0/src/scrollwheel/qtscrollwheel.h
SOURCES += ../embedded-widgets-1.1.0/src/scrollwheel/qtscrollwheel.cpp

include(../embedded-widgets-1.1.0/src/common/common.pri)
#include(../embedded-widgets-1.1.0/src/svgbutton/svgbutton.pri)
#include(../embedded-widgets-1.1.0/src/scrollwheel/scrollwheel.pri)
RESOURCES += \
            ../embedded-widgets-1.1.0/skins/berylsquare_svgbutton.qrc \
            ../embedded-widgets-1.1.0/skins/beryl_scrollwheel.qrc \

FORMS += \
    ldl_dataentry.ui
