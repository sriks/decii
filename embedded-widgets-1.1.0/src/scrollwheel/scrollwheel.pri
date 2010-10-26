INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
HEADERS += $$PWD/qtscrollwheel.h
SOURCES += $$PWD/qtscrollwheel.cpp

QT += svg
QT += gui core
# this is a suprise problem
# if i modify anything in src there are a list of errors
# if GUI is used, not such problems !!!!
