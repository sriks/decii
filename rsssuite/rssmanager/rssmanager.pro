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

win32:DESTDIR = c:/decii/lib
# ensure the specified path has write permission
# sudo chmod 777 <destdir_path>
unix:DESTDIR = /home/decii/lib
# copy header to a common location
win32:header.path = c:/decii/include
unix:header.path = /home/decii/include
header.files = rssmanager.h
INSTALLS += header
build_pass:ALL_DEPS += install_header

# rssparser dependency
QT += xmlpatterns
win32:INCLUDEPATH += c:/decii/include
unix:INCLUDEPATH += /home/decii/include
win32:LIBS += -Lc:/decii/lib
unix:LIBS += -L/home/decii/lib
LIBS += -lrssparser
