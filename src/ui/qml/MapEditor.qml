import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

import "js/Map.js" as Map
import "js/MapEditor.js" as MapEditor

Rectangle {
    id: mapEditor
    property var jobj

    Component.onCompleted: {
        jobj = new MapEditor.MapEditor(ui, map, mapItemTypes);

        mapEditorControls.terrainTypeSelected.connect(jobj.onTerrainTypeSelected.bind(jobj));
        map.mapNodeClicked.connect(jobj.onMapNodeClicked.bind(jobj));

        mapCanvas.onCanvasWindowChanged.connect(mapCanvasWindowChanged);
    }

    function mapCanvasWindowChanged() {
        miniMap.jobj.setWindow(mapCanvas.canvasWindow);
    }

    Rectangle {
        id: mapEditorStatusBar
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: mapEditorSideBar.left
        height: 40

        RowLayout {
            anchors.fill: parent

            Button {
                text: "Save"
                Layout.alignment: Qt.AlignLeft

                onClicked: mapEditor.jobj.saveMap()
            }
        }
    }

    Rectangle {
        id: map

        property var jobj
        signal mapNodeClicked(var mapNode)

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

        Component.onCompleted: {
            jobj = new Map.EditableMap(ui, mapCanvas, mapMouseArea);
            jobj.mapNodeClicked = map.mapNodeClicked;

            miniMap.onWindowPosChanged.connect(jobj.onWindowPosChanged.bind(jobj));
        }

        Canvas {
            id: mapCanvas
            anchors.fill: parent
            onPaint: map.jobj.onPaint(region)
            onImageLoaded: map.jobj.onResourceLoaded()
            tileSize {
                width: 440
                height: 512
            }

            MouseArea {
                id: mapMouseArea

                anchors.fill: parent
                hoverEnabled: true
                acceptedButtons: Qt.LeftButton

                onPressed: map.jobj.onPressed(mouse)
                onReleased: map.jobj.onReleased(mouse)
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
            id: miniMap

            property var jobj
            signal windowPosChanged(var windowPos)

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
                jobj = new Map.MiniMap(ui, miniMapCanvas, miniMapMouseArea);
                jobj.windowPosChanged = miniMap.windowPosChanged;
            }

            Canvas {
                id: miniMapCanvas
                anchors.fill: parent
                onPaint: miniMap.jobj.onPaint(region)
                onImageLoaded: miniMap.jobj.onResourceLoaded()

                MouseArea {
                    id: miniMapMouseArea

                    anchors.fill: parent
                    hoverEnabled: true
                    acceptedButtons: Qt.LeftButton

                    onPressed: miniMap.jobj.onPressed(mouse)
                    onReleased: miniMap.jobj.onReleased(mouse)
                    onPositionChanged: miniMap.jobj.onPositionChanged(mouse)
                }
            }
        }

        Rectangle {
            id: mapEditorControls

            signal terrainTypeSelected(string objectName)

            anchors {
                top: miniMap.bottom
                right: parent.right
                bottom: parent.bottom
                left: parent.left
            }

            TabView {
                id: mapItemTypes
                anchors.fill: parent

                Tab {
                    id: terrainTypeTab
                    title: "Terrain"

                    MapItemSelector {
                        anchors.fill: parent
                        model: ui.map.world.terrainTypes
                        onMapItemSelected: mapEditorControls.terrainTypeSelected(objectName)
                    }
                }
                Tab {
                    id: settlementTab
                    title: "Settlements"

                    MapItemSelector {
                        anchors.fill: parent
                        model: ui.map.world.terrainTypes
                        onMapItemSelected: mapEditorControls.terrainTypeSelected(objectName)
                    }

                }
                Tab {
                    id: unitTab
                    title: "Units"

                    MapItemSelector {
                        anchors.fill: parent
                        model: ui.map.world.terrainTypes
                        onMapItemSelected: mapEditorControls.terrainTypeSelected(objectName)
                    }
                }
            }
        }
    }
}
