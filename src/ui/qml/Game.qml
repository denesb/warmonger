import QtQuick 2.2

import "js/Map.js" as Map

Rectangle {
    id: root

    property var stack

    Rectangle {
        id: statusBar

        height: 24
        anchors {
            top: parent.top
            left: parent.left
            right: sideBar.left
        }
    }

    Rectangle {
        id: sideBar

        width: 256
        anchors {
            top: parent.top
            bottom: parent.bottom
            right: parent.right
        }

        Rectangle {
            id: controls

            height: 24
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }
        }

        MiniMap {
            id: miniMap

            map: W.game

            height: 288
            anchors {
                top: controls.bottom
                left: parent.left
                right: parent.right
            }
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
