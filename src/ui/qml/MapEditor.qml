import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

import "js/EditableMap.js" as EditableMap
import "js/MapEditor.js" as MapEditor

Rectangle {
    id: mapEditor
    property var jobj

    Component.onCompleted: {
        mapEditor.jobj = new MapEditor.MapEditor(ui, map, mapItemTypes);

        mapEditorControls.terrainTypeSelected.connect(mapEditor.jobj.onSelectedTerrainType.bind(mapEditor.jobj));
        map.mapNodeClicked.connect(mapEditor.jobj.onMapNodeClicked.bind(mapEditor.jobj));
    }

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
        property var jobj

        signal mapNodeClicked(var mapNode)

        Component.onCompleted: {
            map.jobj = new EditableMap.EditableMap(ui, mapCanvas);
            map.jobj.mapNodeClicked = map.mapNodeClicked;
        }

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

        Canvas {
            id: mapCanvas
            anchors.fill: parent
            onPaint: map.jobj.paint(region)
            onImageLoaded: map.jobj.onResourceLoaded()

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true

                onClicked: map.jobj.onClicked(mouse)
                onPositionChanged: map.jobj.onPositionChanged(mouse)
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

            signal terrainTypeSelected(string objectName)

            anchors {
                top: mapEditorMiniMap.bottom
                right: parent.right
                bottom: parent.bottom
                left: parent.left
            }

            TabView {
                id: mapItemTypes
                anchors.fill: parent

                Tab {
                    title: "Terrain"

                    Rectangle {
                        ListView {
                            anchors.fill: parent

                            model: ui.map.world.terrainTypes
                            delegate: MapItem {
                                onClicked: mapEditorControls.terrainTypeSelected(model.modelData.objectName)
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
                            }
                        }
                    }
                }
            }
        }
    }
}
