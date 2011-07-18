import QtQuick 1.0

ListView {
    property QtObject info;
    property string viewTitle;
    id: historyInfo;
    model: 1;
    delegate: infoDelegate;

    Component {
        id: infoDelegate;

        Column {
            id: infoColumn
            spacing: 10;
            width: screen.width;
            Text {
                id: title;
                anchors {
                    left: parent.left;
                    right: parent.right
                }
                text: viewTitle;
                font.pixelSize: skin.titleFontSize;
                color: skin.fontColor;
                wrapMode: Text.WordWrap;
                width: parent.width;
                anchors.margins: skin.contentMargin;
            }

            Text {
                id: subTitle;
                anchors {
                    left: parent.left;
                    right: parent.right
                }
                text: info.title();
                font.pixelSize: skin.subTitleFontSize;
                color: skin.fontColor;
                wrapMode: Text.WordWrap;
                horizontalAlignment: Text.AlignHCenter;
                width: parent.width;
                anchors.margins: skin.contentMargin;
            }

            Text {
                id: description;
                anchors.top: subTitle.bottom;
                text: info.description();
                anchors {
                    left: parent.left;
                    right: parent.right
                }
                wrapMode: Text.WordWrap;
                width: parent.width;
                font.pixelSize: skin.contentFontSize;
                color: skin.fontColor;
                anchors.margins: skin.contentMargin;
            }

            Component.onCompleted: {
                console.debug("HistoryInfo.qml onCompleted");
            }
        } // column
    }
}