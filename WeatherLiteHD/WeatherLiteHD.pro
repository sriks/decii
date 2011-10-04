# Add more folders to ship with the application, here
#folder_01.source =
#folder_01.target =
#DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

symbian: {
TARGET.UID3 = 0x200478B9
TARGET.EPOCSTACKSIZE = 0x24000
TARGET.EPOCHEAPSIZE = 0x020000 0x10000000
}
DEFINES += WEATHERSERVICE_WEATHERBUG
CONFIG += qt-components

# Smart Installer package's UID
# This UID is from the protected range and therefore the package will
# fail to install if self-signed. By default qmake uses the unprotected
# range value if unprotected UID is defined for the application and
# 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp

include(../WeatherEngine/WeatherEngine.pri)

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

RESOURCES += \
    resources.qrc

OTHER_FILES += qml/main.qml \
    qml/SelectLocation.qml \
    qml/WeatherSwipeView.qml \
    qml/WeatherConstants.js \
    qml/Day.qml \
    qml/Night.qml \
    qml/WeatherSettings.qml \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog








