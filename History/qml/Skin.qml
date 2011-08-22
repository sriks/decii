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
        property string deleteIcon: "qrc:/images/tb_delete.svg";
        property string shareIcon: "qrc:/images/tb_share5.svg"
        property color fontColor: "#661100"
        property int titleFontSize: 40
        property int subTitleFontSize: 30
        property int contentFontSize: 23
        property int contentMargin: 8
    }
