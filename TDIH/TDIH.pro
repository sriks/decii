# Add files and directories to ship with the application 
# by adapting the examples below.
# file1.source = myfile
# dir1.source = mydir
DEPLOYMENTFOLDERS = # file1 dir1
QT += core gui network xmlpatterns

symbian:TARGET.UID3 = 0x200396A8

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices

# If your application uses the Qt Mobility libraries, uncomment
# the following lines and add the respective components to the 
# MOBILITY variable. 
# CONFIG += mobility
# MOBILITY +=

SOURCES += main.cpp tdih.cpp historyengine.cpp
HEADERS += tdih.h historyengine.h
RESOURCES += resources.qrc

include(../rsssuite/rssmanager/rssmanager.pri)
include(../../git/luavmqt/KonyLib/KSegmentView.pri)
include(../flickcharm/flickcharm.pri)

# Please do not modify the following two lines. Required for deployment.
#include(deployment.pri)
#qtcAddDeployment()
