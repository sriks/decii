import QtQuick 1.0
import Qt.labs.components.native 1.0

Page {
    id: favoritesList;
    property string pageId;
    tools: ToolBarLayout {
            id: toolBarlayout
            ToolButton {
                flat: true
                iconSource: skin.backIcon;
                onClicked: goBack();
            }

            ToolButton {
                flat: true;
                iconSource: skin.menuIcon;
                onClicked: {
                    if (!menu)
                        menu = menuComponent.createObject(root)
                    menu.open()
                    }
            }

            ToolButton {
                flat: true;
                iconSource: skin.closeIcon;
                onClicked: Qt.quit();
            }
        }

    BorderImage {
        id: bkg
        source: skin.bkgImage;
        width: screen.width;
        height: screen.height;
        border.left: 5; border.top: 5
        border.right: 5; border.bottom: 5
    }

    Text {
        id: title;
        anchors {
            left: parent.left;
            right: parent.right
        }
        text: "MY FAVORITES"
        font.pixelSize: skin.titleFontSize;
        color: skin.fontColor;
        wrapMode: Text.WordWrap;
        width: parent.width;
        anchors.margins: skin.contentMargin;
    }

    ListView {
        id: favList
        width: screen.width;
        height: 400;
        anchors.top: title.bottom
        anchors.topMargin: 20;
        spacing: 10;
        model: engine.favorites();
        delegate:
            Rectangle {
                width: parent.width - 10;
                height: favItemText.height + 20;
                anchors.horizontalCenter: parent.horizontalCenter;
                border.width: 2;
                border.color: "#FF44FFAA";
                color: "transparent";
                radius: 10;
                smooth: true;

                Text {
                    id: favItemText;
                    text: modelData
                    width: parent.width - 10;
                    wrapMode: Text.WordWrap;
                    color: skin.fontColor;
                    font.pixelSize: skin.contentFontSize;
                    anchors.margins: 5
                    anchors.centerIn: parent;
                }

                MouseArea {
                    anchors.fill: parent;
                    onClicked: {
                        console.debug("favlist.qml onclicked at "+index);
                        loadFavorite(index);
                    }
                }
           }
       }

    Component.onCompleted: {
        pageId = "favlist";
        console.debug("favlist.qml: formloaded:"+pageId);
        console.debug("favlist.qml onComplete")
    }

    Component.onDestruction: {
        console.debug("favlist.qml onDestruction:"+pageId);
    }

} //page
