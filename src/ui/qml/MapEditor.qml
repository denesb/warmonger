import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

import "js/MapEditor.js" as MapEditor

Rectangle {
    Rectangle {
        id: mapEditorStatusBar
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: mapEditorSideBar.left
        height: 40
        color: "green"
    }

    Rectangle {
        id: map

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

        property var mapObject

        Component.onCompleted: {
            map.mapObject = new MapEditor.EditableMap(ui, mapCanvas)
        }
        Canvas {
            id: mapCanvas
            anchors.fill: parent
            onPaint: map.mapObject.paint(region)
            onImageLoaded: map.mapObject.onResourceLoaded()

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true

                onClicked: map.mapObject.onClick(mouse)
                onPositionChanged: map.mapObject.onPositionChange(mouse)
            }
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

                            model: ui.map.world.terrainTypes
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

                            model: ui.map.world.terrainTypes
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

                            model: ui.map.world.terrainTypes
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
