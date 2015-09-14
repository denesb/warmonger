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
                left: parent.left
                top: parent.top
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

    }

    GameMap {
        id: map

        game: W.game

        anchors {
            top: statusBar.bottom
            bottom: parent.bottom
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
}
