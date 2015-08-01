import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

import "js/MapEditor.js" as MapEditor

Rectangle {
    Component.onCompleted: {
    }
    Rectangle {
        id: mapEditorStatusBar
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: mapEditorSideBar.left
        height: 40
        color: "green"
    }

    Map {
        id: mapEditorMap
        anchors {
            left: parent.left
            top: mapEditorStatusBar.bottom
            right: mapEditorSideBar.left
            bottom: parent.bottom
        }

        border {
            width: 1
            color: "black"
        }
    }

    Rectangle {
        id: mapEditorSideBar
        anchors {
            top: parent.top
            right: parent.right
            bottom: parent.bottom
        }

        width: 256
        color: "blue"

        Rectangle {
            id: mapEditorMiniMap
            anchors {
                left: parent.left
                top: parent.top
                right: parent.right
            }

            height: 288
            color: "yellow"
        }

        Rectangle {
            id: mapEditorControls
            anchors {
                top: mapEditorMiniMap.bottom
                right: parent.right
                bottom: parent.bottom
                left: parent.left
            }

            TabView {
                anchors.fill: parent

                Tab {
                    title: "Terrain"

                    Rectangle {
                        ListView {
                            anchors.fill: parent

                            model: warmonger.map.world.terrainTypes
                            delegate: MapItem {
                                onClicked: MapEditor.selectMapItem(model.modelData.objectName)
                            }
                        }
                    }
                }
                Tab {
                    title: "Settlements"

                    Rectangle {
                        ListView {
                            anchors.fill: parent

                            model: warmonger.map.world.settlementTypes
                            delegate: MapItem {
                                onClicked: MapEditor.selectMapItem(model.modelData.objectName)
                            }
                        }
                    }
                }
                Tab {
                    title: "Units"

                    Rectangle {
                        ListView {
                            anchors.fill: parent

                            model: warmonger.map.world.unitTypes
                            delegate: MapItem {
                                onClicked: MapEditor.selectMapItem(model.modelData.objectName)
                            }
                        }

                    }
                }
            }
        }
    }
}
