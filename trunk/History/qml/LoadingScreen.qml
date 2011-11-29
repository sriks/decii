import QtQuick 1.0
import com.nokia.symbian 1.0
import "HistoryConstants.js" as HistoryConstants;

Page {
    id: loadingScreen;
    property string pageId;
    tools: ToolBarLayout {
            id: toolBarlayout
            Component.onCompleted:
                createBackToolButton(toolBarlayout)
    }

    BorderImage {
        id: name
        source: skin.bkgImage;
        width: screen.width;
        height: screen.height;
        border.left: 5; border.top: 5
        border.right: 5; border.bottom: 5
    }

    BusyIndicator {
         id: busyIndicator;
         running: true
         width: 60
         height: 60
         anchors.centerIn: parent;
    }

    Text {
        anchors.centerIn: parent;
        text: HistoryConstants.loadingText;
        color: skin.fontColor;
        font.pixelSize: skin.subTitleFontSize;
    }

    Component.onCompleted: {
        pageId = HistoryConstants.loadingScreenPageId;
        busyIndicator.running = true;
    }
}
