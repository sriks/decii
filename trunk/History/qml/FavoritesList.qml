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

    ListView {
        id: favList
        width: screen.width;
        spacing: 10;
        anchors.fill: parent
        model: 1
        delegate: favListView

        Component {
            id: favListView;
            Column {
                id: favListColumn
                width: screen.width;
            Text {
                id: title;
                text: "MY FAVORITES"
                font.pixelSize: skin.titleFontSize;
                color: skin.fontColor;
                wrapMode: Text.WordWrap;
                horizontalAlignment: Text.AlignHCenter;
                width: parent.width;
                anchors.margins: skin.contentMargin;
                }

            Repeater {
                id: listItemRepeater
                model: engine.favorites();
                delegate: listItem;
            }

            Component {
                id: listItem
            Item {
                width: screen.width;
                height: favItemText.height + 25;
                anchors.horizontalCenter: parent.horizontalCenter;

                Rectangle {
                    width: parent.width-10;
                    height: 2;
                    border.color: "white";
                    anchors.horizontalCenter: parent.horizontalCenter;
                }

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

                Rectangle {
                    width: parent.width-10;
                    height: 2;
                    border.color: "white";
                    anchors.horizontalCenter: parent.horizontalCenter;

                }

                MouseArea {
                    anchors.fill: parent;
                    onClicked: loadFavorite(index);
                    }
            } }

            } // column
        }// component
    }// listview\

    Component.onCompleted: {
        pageId = "favlist";
        console.debug("favlist.qml: formloaded:"+pageId);
        console.debug("favlist.qml onComplete")
    }

    Component.onDestruction: {
        console.debug("favlist.qml onDestruction:"+pageId);
    }
}



