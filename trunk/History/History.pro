# Add more folders to ship with the application, here
folder_01.source = qml
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

symbian:TARGET.UID3 = 0x200478B8

# Allow network access on Symbian
symbian {
TARGET.CAPABILITY += LocalServices ReadUserData WriteUserData NetworkServices UserEnvironment ReadDeviceData WriteDeviceData
symbian:ICON = historyappicon.svg
DEPLOYMENT.installer_header=0xA000D7CE
vendorinfo = "%{\"Dreamcode\"}" ":\"Dreamcode\""

my_deployment.pkg_prerules = vendorinfo
DEPLOYMENT += my_deployment
}


# Define QMLJSDEBUGGER to allow debugging of QML in debug builds
# (This might significantly increase build time)
# DEFINES += QMLJSDEBUGGER

# If your application uses the Qt Mobility libraries, uncomment
# the following lines and add the respective components to the 
# MOBILITY variable. 
CONFIG += mobility
MOBILITY += messaging

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

RESOURCES += \
    resources.qrc

include(HistoryEngine/HistoryEngine.pri)

OTHER_FILES += qml/main.qml \
    qml/HistoryInfo.qml \
    qml/Today.qml \
    qml/Skin.qml \
    qml/FavoritesList.qml \
    qml/Favorite.qml \
    qml/DefaultToolButton.qml \
    qml/HistoryConstants.js