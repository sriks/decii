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

# rssparser dependency start
QT += xmlpatterns
INCLUDEPATH += ../../rssparser
win32: {
    LIBS += -Lc:/mylibs
    LIBS += -lrssparser
}
unix: {
    LIBS += -L/usr/lib
    LIBS += -lrssparser
}

# rssmanager dependency start
QT += xmlpatterns
INCLUDEPATH += ../../rssmanager
win32: {
    LIBS += -Lc:/mylibs
    LIBS += -lrssmanager
}
unix: {
    LIBS += -L/usr/lib
    LIBS += -lrssmanager
}


