#-------------------------------------------------
#
# Project created by QtCreator 2010-09-09T17:18:02
#
#-------------------------------------------------

QT       += core xmlpatterns network
QT       -= gui

TARGET = rssmanager
CONFIG   += dll
CONFIG   -= app_bundle
TEMPLATE = lib
SOURCES += main.cpp \
    rssmanager.cpp \
    feedprofile.cpp

HEADERS += \
    rssmanager.h \
    feedprofile.h

# rssparser dependency
QT += xmlpatterns
INCLUDEPATH += ../rssparser
win32: {
    LIBS += -Lc:/mylibs
    LIBS += -lrssparser
}
unix: {
    LIBS += -L/usr/lib
    LIBS += -lrssparser
}

# Destination
win32:{
DESTDIR = c:/mylibs
}

unix:{
# ensure the specified path has write permission
# sudo chmod 777 <destdir_path>
DESTDIR = /usr/lib
}
