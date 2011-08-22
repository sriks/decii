import QtQuick 1.0
import Qt.labs.components.native 1.0
import com.nokia.extras 1.0
import "HistoryConstants.js" as HistoryConstants;

Page {
    id: fav;
    property string pageId;
    tools: ToolBarLayout {
            id: toolBarlayout
            Component.onCompleted: {
                createBackToolButton(toolBarlayout);
                createDeleteFavToolButton(toolBarlayout)
                createShareToolButton(toolBarlayout);
                createMenuToolButton(toolBarlayout);
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
        pageId = HistoryConstants.favPageId;
        infoLoader.source = "HistoryInfo.qml";
        infoLoader.item.viewTitle = HistoryConstants.myFavoritesText;
        infoLoader.item.info = engine.favorite(currentFavIndex);
    }

} //page

