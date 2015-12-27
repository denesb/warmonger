import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import Warmonger 1.0

Rectangle {
    id: root

    property var stack

    Component {
        id: recruitUnits

        RecruitUnits {
            anchors.centerIn: parent
        }
    }

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

        ColumnLayout {
            anchors.fill: parent

            Rectangle {
                Layout.preferredHeight: 256
                Layout.fillWidth: true

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

            GameItemInfo {
                Layout.fillHeight: true
                Layout.fillWidth: true

                mapNode: map.focusedMapNode
                settlement: map.focusedSettlement
                unit: map.focusedUnit
            }

            Rectangle {
                Layout.preferredHeight: 30
                Layout.fillWidth: true

                Button {
                    text: "Recruit"

                    visible: {
                        var n = map.focusedMapNode;
                        var s = map.focusedSettlement;
                        var u = map.focusedUnit;
                        n && s && u && (u.rankName === "Officer" || u.rankName === "Leader");
                    }

                    onClicked: {
                        recruitUnits.createObject(
                            mapOverlay,
                            {
                                "settlement": map.focusedSettlement
                            }
                        );
                        mapOverlay.visible = true;
                        map.enabled = false;
                    }
                }
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

        Rectangle {
            id: mapOverlay

            anchors.fill: parent
            z: 1
            visible: false
            color: "transparent"
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
