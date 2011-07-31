import QtQuick 1.0
import Qt.labs.components.native 1.0

    Item {
        id: auroraSkin
        property string bkgImage: "qrc:/images/bkg_h4.jpg";
        property string menuIcon: "qrc:/images/tb_menu.svg";
        property string homeIcon: "qrc:/images/tb_home.svg";
        property string favIcon: "qrc:/images/tb_favorite.svg";
        property string backIcon: "qrc:/images/tb_back.svg"
        property string closeIcon: "qrc:/images/tb_close.svg";
        property string aboutText: "History\nA quality product by Dreamcode.\nConceptualized and developed at Dreamcode Device Labs,2011.\nIcon courtesy openclipart.com"
        property string netErrorText: "Opps! Some problem with your network connection. Cannot continue.\n\n";
        property color fontColor: "#661100"
        property int titleFontSize: 40
        property int subTitleFontSize: 30
        property int contentFontSize: 22
        property int contentMargin: 8
    }
