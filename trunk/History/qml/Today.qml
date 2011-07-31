import QtQuick 1.0
import Qt.labs.components.native 1.0
import com.nokia.extras 1.0

Page {
    id: today;
    property string pageId;
    tools: ToolBarLayout {
            id: toolBarlayout
            ToolButton {
                flat: true
                iconSource: skin.favIcon;
                onClicked: {
                             if(!engine.saveAsFavorite())
                                 banner.text = "Unable to save as favorite";
                             banner.open();
                           }
            }

            ToolButton {
                iconSource: skin.menuIcon;
                onClicked: {
                    if (!menu)
                        menu = menuComponent.createObject(root)
                    menu.open()
                }
            }

            ToolButton {
                iconSource: skin.closeIcon;
                onClicked: Qt.quit();

            }
        }

    InfoBanner {
         id: banner
         text: "Saved as favorite"
         iconSource: skin.favIcon;
         timeout: 3000;
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
        var info = engine.historyInfo();
        console.debug("main.qml "+arguments.callee.name);
        infoLoader.source = "HistoryInfo.qml";
        infoLoader.item.viewTitle = "THIS DAY IN HISTORY";
        infoLoader.item.info = engine.historyInfo();
    }

    Component.onDestruction: {
        console.debug("today.qml onDestruction:"+pageId);
    }

} //page

