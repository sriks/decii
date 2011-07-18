import QtQuick 1.0
import Qt.labs.components.native 1.0

Window {
    id: root;
    property QtObject skin;
    property Menu menu;
    property int currentFavIndex: 0;
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
        id: aboutDialog;
        message: "History\nA quality product by Dreamcode.\nConceptualized and developed at Dreamcode Device Labs,2011.\nIcon courtesy openclipart.com"
        acceptButtonText: "OK"
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
                    onClicked: aboutDialog.open();
                }

                MenuItem { text: "Quit"; onClicked: Qt.quit() }
            }
        }
    }

    Component.onCompleted: {
        var skinComponent = Qt.createComponent("Skin.qml");
        if(Component.Ready  == skinComponent.status) {
            skin = skinComponent.createObject(root); // if not created with parent, skin is not getting recognised in device.
            console.debug("main.qml skin created: "+skin.id);
        }
        else
            console.debug("main.qml cannot create skin:"+skinComponent.errorString());
        engine.start();
    }

    function onUpdateAvailable() {
        loadToday();
    }

    function loadToday() {
        pageStack.push(Qt.resolvedUrl("Today.qml"));
    }

    function loadFavList() {
        pageStack.push(Qt.resolvedUrl("FavoritesList.qml"));
    }

    function loadFavorite(favIndex) {
        var fav = engine.favorite(favIndex);
        console.debug("main.qml loadfavorite: "+fav.title());
        pageStack.push(Qt.resolvedUrl("Favorite.qml"));
    }

    function goBack() {
        pageStack.pop();
    }
}

