# Add files and directories to ship with the application 
# by adapting the examples below.
# file1.source = myfile
# dir1.source = mydir
DEPLOYMENTFOLDERS = # file1 dir1

symbian:TARGET.UID3 = 0xE43054AE

# Allow network access on Symbian
#symbian:TARGET.CAPABILITY += NetworkServices

# If your application uses the Qt Mobility libraries, uncomment
# the following lines and add the respective components to the 
# MOBILITY variable. 
CONFIG += mobility
MOBILITY += sensors

SOURCES += main.cpp mainwindow.cpp\
    MoveToZoom.cpp \
    testview.cpp
HEADERS += MoveToZoom.h mainwindow.h \
    testview.h

# Please do not modify the following two lines. Required for deployment.
include(deployment.pri)
qtcAddDeployment()
