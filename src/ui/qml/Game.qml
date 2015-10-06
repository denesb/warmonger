import QtQuick 2.2
import QtQuick.Controls 1.1
import Warmonger 1.0

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

            Button {
                text: "Quit"

                onClicked: {
                    W.closeGame();
                    root.stack.pop();
                    root.stack.pop();
                }
            }
        }

        MiniMap {
            id: miniMap

            /*map: W.game*/
            window: map.window

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

    Rectangle {
        id: mapWrapper

        anchors {
            top: statusBar.bottom
            bottom: infoBar.top
            left: parent.left
            right: sideBar.left
        }

        GameMap {
            id: map

            game: W.game
            //windowPos: miniMap.windowPos

            anchors {
                fill: parent
                margins: 1
            }
        }
    }

    InfoBar {
        id: infoBar

        world: W.world
        mapNode: map.currentMapNode
        pos: map.currentPos
        settlement: map.currentSettlement
        unit: map.currentUnit

        anchors {
            bottom: parent.bottom
            left: parent.left
            right: sideBar.left
        }
        height: 24
    }
}
