import QtQuick 2.2

import "js/Map.js" as Map

Rectangle {
    id: root

    property var stack

    Rectangle {
        id: statusBar

        anchors {
            top: parent.top
            left: parent.left
            right: sideBar.left
        }
        height: 40
    }

    Rectangle {
        id: sideBar

        anchors {
            top: parent.top
            bottom: parent.bottom
            right: parent.right
        }

        width: 256

        MiniMap {
            id: miniMap

            map: W.game

            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }
            height: 288
            border {
                width: 1
                color: "black"
            }

            Component.onCompleted: {
                map.windowChanged.connect(miniMap.onMapWindowChanged);
            }

            function onMapWindowChanged(mapWindow) {
                miniMap.window = mapWindow;
            }
        }

        GameItemInfo {
            id: gameItemInfo

            world: W.world
            mapNode: map.focusedMapNode

            anchors {
                top: miniMap.bottom
                bottom: parent.bottom
                left: parent.left
                right: parent.right
            }
        }
    }

    GameMap {
        id: map

        game: W.game

        anchors {
            top: statusBar.bottom
            bottom: infoBar.top
            left: parent.left
            right: sideBar.left
        }
        border {
            width: 1
            color: "black"
        }

        Component.onCompleted: {
            miniMap.windowPosChanged.connect(map.onMiniMapWindowPosChanged);
        }

        function onMiniMapWindowPosChanged(miniMapWindowPos) {
            map.windowPos = miniMapWindowPos;
        }
    }

    InfoBar {
        id: infoBar

        world: W.world
        mapNode: map.currentMapNode

        anchors {
            bottom: parent.bottom
            left: parent.left
            right: sideBar.left
        }
        height: 24
    }
}
