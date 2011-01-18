INCLUDEPATH +=  $$PWD
DEPENDPATH += $$PWD
HEADERS += $$PWD/segment.h \
           $$PWD/segmentwidget.h \
           $$PWD/segmentview.h
SOURCES += $$PWD/segment.cpp \
           $$PWD/segmentwidget.cpp \
           $$PWD/segmentview.cpp

include(../flickcharm/flickcharm.pri)
