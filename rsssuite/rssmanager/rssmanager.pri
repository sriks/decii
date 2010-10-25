
QT += xmlpatterns
INCLUDEPATH +=  $$PWD
win32: {
     LIBS += -Lc:/decii/lib
}
LIBS += -lrssmanager
LIBS += -lrssparser

