import QtQuick 1.0
import com.nokia.symbian 1.0
import "HistoryConstants.js" as HistoryConstants;

Page {
    id: today;
    property string pageId;
    tools: ToolBarLayout {
            id: toolBarlayout
            Component.onCompleted: {
                createBackToolButton(toolBarlayout);
                createSaveAsFavToolButton(toolBarlayout);
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
        pageId = HistoryConstants.todayPageId;
        var info = engine.historyInfo();
        infoLoader.source = "HistoryInfo.qml";
        infoLoader.item.viewTitle = HistoryConstants.todayTitleText;
        infoLoader.item.info = engine.historyInfo();
    }

} //page

