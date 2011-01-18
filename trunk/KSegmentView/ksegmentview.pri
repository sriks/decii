INCLUDEPATH +=  $$PWD
DEPENDPATH += $$PWD
HEADERS += $$PWD/ksegment.h \
           $$PWD/ksegmentwidget.h \
           $$PWD/ksegmentview.h
SOURCES += $$PWD/ksegment.cpp \
           $$PWD/ksegmentwidget.cpp \
           $$PWD/ksegmentview.cpp

include(../flickcharm/flickcharm.pri)
