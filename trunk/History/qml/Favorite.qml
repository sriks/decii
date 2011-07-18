import QtQuick 1.0
import Qt.labs.components.native 1.0
import com.nokia.extras 1.0

Page {
    id: fav;
    property string pageId;
    tools: ToolBarLayout {
            id: toolBarlayout

            ToolButton {
                flat: true
                iconSource: skin.backIcon
                onClicked: goBack();
            }

            ToolButton {
                flat: true
                iconSource: skin.closeIcon;
                onClicked: Qt.quit();
            }
        }

    BorderImage {
        id: name
        source: skin.bkgImage;
        width: screen.width;
        height: screen.height;
        border.left: 5; border.top: 5
        border.right: 5; border.bottom: 5
    }

    Loader {
      id: infoLoader;
      anchors.fill: parent;
    }

    Component.onCompleted: {
        pageId = "today";
        console.debug("today.qml: formloaded:"+pageId);
        console.debug("Today.qml onComplete")

        var info = engine.historyInfo();
        console.debug("main.qml "+arguments.callee.name);
        infoLoader.source = "HistoryInfo.qml";
        infoLoader.item.viewTitle = "MY FAVORITES";
        infoLoader.item.info = engine.favorite(currentFavIndex);
    }

    Component.onDestruction: {
        console.debug("today.qml onDestruction:"+pageId);
    }

} //page

