import QtQuick 1.0
import Qt.labs.components.native 1.0

Window {
    id: root;
    property QtObject skin;
    property Menu menu;
    property int currentFavIndex: 0;
    signal error(string errorString)
    PageStack {
        id: pageStack
        visible: true
        anchors.top: statusBar.bottom
        anchors.bottom: sharedToolBar.top;
        toolBar: sharedToolBar;
    }

    StatusBar {
         id: statusBar
         anchors.top: root.top
    }


    ToolBar {
        id: sharedToolBar
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
    }

    QueryDialog {
        id: dialog;
        property bool closeOnAccept;
        titleText: "History";
        onAccepted: {
            if(dialog.closeOnAccept)
                Qt.quit();
        }
    }

    BusyIndicator {
         id: busyIndicator;
         running: true
         width: 60
         height: 60
         anchors.centerIn: parent;
         function stop() {
             busyIndicator.running = false;
             busyIndicator.visible = false;
         }
         function start() {
             busyIndicator.running = true;
             busyIndicator.visible = true;
         }
    }

    Button {
        id: defaultCloseTools;
        iconSource: skin.closeIcon;
        anchors.top: busyIndicator.bottom;
        anchors.topMargin: 10;
        anchors.horizontalCenter: parent.horizontalCenter;
        text: "Close";
        onClicked: Qt.quit();
    }

    Component {
        id: menuComponent
        Menu {
            content: MenuLayout {
                MenuItem {
                    text: "Favorites"
                    enabled: (engine.favoritesCount())?(true):(false);
                    onClicked: loadFavList()
                }

                MenuItem {
                    text: "About"
                    onClicked: showAboutDialog();
                }

                MenuItem { text: "Quit"; onClicked: Qt.quit() }
            }
        }
    }

    Component.onCompleted: {
        var skinComponent = Qt.createComponent("Skin.qml");
        if(Component.Ready  == skinComponent.status)
            skin = skinComponent.createObject(root); // if not created with parent, skin is not getting recognised in device.
        else
            busyIndicator.start();
        engine.start();
    }

    onError: {
        console.debug("error string:"+errorString);
        dialog.message = skin.netErrorText+errorString;
        dialog.acceptButtonText = "I pity, close app.";
        dialog.closeOnAccept = true;
        dialog.open();
    }

    function showAboutDialog() {
        dialog.message = skin.aboutText;
        dialog.acceptButtonText = "OK";
        dialog.open();
    }

    function onUpdateAvailable() {
        defaultCloseTools.visible = false;
        busyIndicator.stop();
        loadToday();
    }

    function loadToday() {
        pageStack.push(Qt.resolvedUrl("Today.qml"));
    }

    function loadFavList() {
        pageStack.push(Qt.resolvedUrl("FavoritesList.qml"));
    }

    function loadFavorite(favIndex) {
        currentFavIndex = favIndex;
        var fav = engine.favorite(favIndex);
        console.debug("main.qml loadfavorite: "+fav.title());
        pageStack.push(Qt.resolvedUrl("Favorite.qml"));
    }

    function goBack() {
        pageStack.pop();
    }
}

