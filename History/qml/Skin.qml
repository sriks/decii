import QtQuick 1.0
import Qt.labs.components.native 1.0

    Item {
        id: auroraSkin
        property string bkgImage: "qrc:/images/bkg.png";
        property string menuIcon: "qrc:/images/tb_menu.svg";
        property string homeIcon: "qrc:/images/tb_home.svg";
        property string favIcon: "qrc:/images/tb_favorite.svg";
        property string backIcon: "qrc:/images/tb_back.svg"
        property string closeIcon: "qrc:/images/tb_close.svg";
        property color fontColor: "#FFFFFF"
        property int titleFontSize: 40
        property int subTitleFontSize: 30
        property int contentFontSize: 20
        property int contentMargin: 8
    }
