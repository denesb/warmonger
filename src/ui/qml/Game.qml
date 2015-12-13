import QtQuick 2.2
import QtQuick.Controls 1.1
import Warmonger 1.0

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

        Rectangle {
            id: miniMapWrapper

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

            MiniMap {
                id: miniMap

                map: W.game
                windowPos: map.windowPos
                windowSize: map.windowSize

                anchors {
                    fill: parent
                    margins: 1
                }
            }
        }

        StackView {
            id: gameControlStack

            anchors {
                top: miniMapWrapper.bottom
                bottom: parent.bottom
                left: parent.left
                right: parent.right
            }
            initialItem: gameItemInfo

            Component {
                id: gameItemInfo

                GameItemInfo {
                    mapNode: map.focusedMapNode
                    settlement: map.focusedSettlement
                    unit: map.focusedUnit

                    onSettlementClicked: {
                        gameControlStack.push({
                            item: Qt.createComponent("RecruitUnits.qml"),
                            properties: {
                                stack: gameControlStack,
                                settlement: map.focusedSettlement
                            }
                        });
                    }
                }
            }
        }

        Rectangle {
            id: turnInfo

            height: 24
            anchors {
                bottom: parent.bottom
                left: parent.left
                right: parent.right
            }

            Text {
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: parent.left
                }

                text: "Turn " + W.game.turn
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
            windowPos: miniMap.windowPos

            anchors {
                fill: parent
                margins: 1
            }
        }
    }

    InfoBar {
        id: infoBar

        mapNode: map.currentMapNode
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
