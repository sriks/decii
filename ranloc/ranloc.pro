#-------------------------------------------------
#
# Project created by QtCreator 2010-11-13T21:38:50
#
#-------------------------------------------------

QT       += core gui network xmlpatterns webkit

TARGET = ranloc
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    locationengine.cpp

HEADERS  += mainwindow.h \
    locationengine.h

FORMS    += mainwindow.ui

include(../custombutton/custombutton.pri)

#CONFIG += mobility
#MOBILITY =


symbian {
# work around for bug QHttp host not found
# http://bugreports.qt.nokia.com/browse/QTBUG-8687
# workaround found at http://api.witinside.net/mobile/qt/symbian-s60/HostNotFoundError.php
  HEADERS += S60QHttp.h \
   S60QHttpPrivate.h
  SOURCES += S60QHttp.cpp \
   S60QHttpPrivate.cpp

  LIBS += -lcone \
   -leikcore \
   -lavkon \
   -lbafl \
   -lInetProtUtil \ # for UriParser
   -lhttp \ # for HTTP Client API
   -lecom \ # for HTTP Client API
   -lesock \ # for RSocketServ
   -lcommdb
TARGET.CAPABILITY = NetworkServices

TARGET.UID3 = 0xec6ff120
# TARGET.CAPABILITY +=
TARGET.EPOCSTACKSIZE = 0x14000
TARGET.EPOCHEAPSIZE = 0x020000 0x800000
}

