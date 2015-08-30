import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

import "js/Map.js" as Map

Rectangle {
    id: mapEditor

    Component.onCompleted: {
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
                Layout.preferredWidth: width

                onClicked: W.map.save()
            }

            Rectangle {
                Layout.alignment: Qt.AlignLeft
                Layout.fillWidth: true
            }

            Button {
                text: "Select"
                Layout.alignment: Qt.AlignRight
                Layout.preferredWidth: width

                onClicked: {
                    editModes.currentIndex = 0;
                    map.jobj.setEditMode(Map.EditableMap.SelectMode);
                }
            }

            Button {
                text: "Create Map Nodes"
                Layout.alignment: Qt.AlignRight
                Layout.preferredWidth: width

                onClicked: {
                    editModes.currentIndex = 1;
                    map.jobj.setEditMode(Map.EditableMap.CreateMapNodeMode);
                }
            }

            Button {
                text: "Create Settlements"
                Layout.alignment: Qt.AlignRight
                Layout.preferredWidth: width

                onClicked: {
                    editModes.currentIndex = 2;
                    map.jobj.setEditMode(Map.EditableMap.CreateSettlementMode);
                }
            }

            Button {
                text: "Create Units"
                Layout.alignment: Qt.AlignRight
                Layout.preferredWidth: width

                onClicked: {
                    editModes.currentIndex = 3;
                    map.jobj.setEditMode(Map.EditableMap.CreateUnitMode);
                }
            }

            Button {
                text: "Edit Map Nodes"
                Layout.alignment: Qt.AlignRight
                Layout.preferredWidth: width

                onClicked: {
                    editModes.currentIndex = 4;
                    map.jobj.setEditMode(Map.EditableMap.EditMapNodeMode);
                }
            }

            Button {
                text: "Edit Settlement"
                Layout.alignment: Qt.AlignRight
                Layout.preferredWidth: width

                onClicked: {
                    editModes.currentIndex = 5;
                    map.jobj.setEditMode(Map.EditableMap.EditSettlementMode);
                }
            }
        }
    }

    Rectangle {
        id: map

        property var jobj
        signal selectMapItems(var mapNode, var settlement)
        signal editMapNode(var mapNode)
        signal editSettlement(var settlement)

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
            jobj = new Map.EditableMap(W, mapCanvas, mapMouseArea);
            jobj.onSelectMapItems = map.selectMapItems;
            jobj.onEditMapNode = map.editMapNode;
            jobj.onEditSettlement = map.editSettlement;

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
                jobj = new Map.MiniMap(W, miniMapCanvas, miniMapMouseArea);
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
            signal unitTypeSelected(string objectName)
            signal settlementTypeSelected(string objectName)

            onTerrainTypeSelected: map.jobj.setTerrainType(objectName)
            onSettlementTypeSelected: map.jobj.setSettlementType(objectName)
            onUnitTypeSelected: map.jobj.setUnitType(objectName)

            anchors {
                top: miniMap.bottom
                right: parent.right
                bottom: parent.bottom
                left: parent.left
            }

            TabView {
                id: editModes
                anchors.fill: parent
                tabsVisible: false

                Tab {
                    id: selectMapItem
                    title: "Select"

                    MapItemsInfo {
                        id: mapItemsInfo
                        anchors.fill: parent

                        function onSelectMapItems(mapNode, settlement) {
                            mapItemsInfo.mapNode = mapNode;
                            mapItemsInfo.settlement = settlement;
                        }

                        Component.onCompleted: {
                            map.selectMapItems.connect(mapItemsInfo.onSelectMapItems);
                        }
                    }
                }

                Tab {
                    id: createMapNode
                    title: "Create MapNode"

                    MapItemSelector {
                        anchors.fill: parent
                        model: W.map.world.terrainTypes
                        onMapItemSelected: mapEditorControls.terrainTypeSelected(objectName)
                    }
                }

                Tab {
                    id: createSettlement
                    title: "Create Settlement"

                    MapItemSelector {
                        anchors.fill: parent
                        model: W.map.world.settlementTypes
                        onMapItemSelected: mapEditorControls.settlementTypeSelected(objectName)
                    }

                }
                Tab {
                    id: createUnit
                    title: "Create Unit"

                    MapItemSelector {
                        anchors.fill: parent
                        model: W.map.world.unitTypes
                        onMapItemSelected: mapEditorControls.unitTypeSelected(objectName)
                    }
                }

                Tab {
                    id: editMapNode
                    title: "Edit MapNode"

                    MapNodeEdit {
                        id: mapNodeEdit
                        anchors.fill: parent

                        function onEditMapNode(mapNode) {
                            mapNodeEdit.mapNode = mapNode;
                        }

                        Component.onCompleted: {
                            map.editMapNode.connect(mapNodeEdit.onEditMapNode);
                        }
                    }
                }

                Tab {
                    id: editSettlement
                    title: "Edit Settlement"

                    SettlementEdit {
                        id: settlementEdit
                        anchors.fill: parent

                        function onEditSettlement(settlement) {
                            settlementEdit.settlement = settlement;
                        }

                        Component.onCompleted: {
                            map.editSettlement.connect(settlementEdit.onEditSettlement);
                        }
                    }
                }

            }
        }
    }
}
