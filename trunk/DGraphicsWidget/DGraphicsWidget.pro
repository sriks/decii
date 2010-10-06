#-------------------------------------------------
#
# Project created by QtCreator 2010-09-23T12:17:47
#
#-------------------------------------------------

#TEMPLATE = app
TEMPLATE = lib
QT       += core gui
TARGET = DGraphicsWidget

contains( TEMPLATE, lib ) {
    CONFIG  += dll
    win32:{
    DESTDIR = c:/decii/lib
    headers.path = c:/decii/include/dgraphicswidget
    }
    headers.files += dgraphicswidget.h
    headers.files += dgraphicsview.h
    headers.files += dgraphicspixmapwidget.h
    headers.files += dgraphicstextwidget.h
    headers.files += dgraphicswidget.pri
    INSTALLS += headers
    PRE_TARGETDEPS += install_headers
}

SOURCES += main.cpp\
    dgraphicstextwidget.cpp \
    dgraphicspixmapwidget.cpp \
    dgraphicswidget.cpp \
    dgraphicstitlewidget.cpp \
    dgraphicsview.cpp

HEADERS  += dgraphicswidget.h \
    dgraphicstextwidget.h \
    dgraphicspixmapwidget.h \
    dgraphicstitlewidget.h \
    dgraphicsview.h \
    common.h

RESOURCES += \
    resource.qrc
