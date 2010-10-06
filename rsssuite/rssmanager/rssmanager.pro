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

# rsssuite dependency start
# Any client willing to use rsssuite can copy the following lines in .pro
#####################################################
# For unix systems, dlls and headers are copied to standard /usr/lib and /usr/include respectively
# hence they need not be mentioned in unix targets.

QT += xmlpatterns
win32: {
    INCLUDEPATH += c:/decii/include/rsssuite
    LIBS += -Lc:/decii/lib
        }
    LIBS += -lrssparser
#####################################################

# In linux ensure the specified path has write permission
# sudo chmod 777 /usr/lib
# sudo chmod 777 /usr/include
win32:{DLLDESTDIR = c:/decii/lib
headers.path = c:/decii/include/rsssuite}
unix:{DESTDIR = /usr/lib
headers.path = /usr/include/rsssuite}
headers.files += rssmanager.h
headers.files += rssmanager.pri
INSTALLS += headers
PRE_TARGETDEPS += install_headers

