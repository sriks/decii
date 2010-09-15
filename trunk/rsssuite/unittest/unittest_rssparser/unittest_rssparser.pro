# -------------------------------------------------
# Project created by QtCreator 2010-08-25T11:13:26
# -------------------------------------------------
QT += testlib
TARGET = unittest_rssparser
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
HEADERS += test_rssparser.h
SOURCES += main.cpp \
    test_rssparser.cpp

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

RESOURCES += \
    resource.qrc
