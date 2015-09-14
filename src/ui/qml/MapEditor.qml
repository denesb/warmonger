import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.0

import "js/Map.js" as Map

Rectangle {
    id: root

    property var stack

    FileDialog {
        id: fileDialog
        visible: false

        title: "Select Map"

        nameFilters: ["Warmonger Map Definition files (*.wmd)", "All files (*)"]
        selectExisting: true
        selectFolder: false
        selectMultiple: false

        onAccepted: {
            W.loadMapFromUrl(fileDialog.fileUrl);
        }
        onRejected: {
            console.log("Canceled")
        }
    }

    Rectangle {
        id: statusBar
        anchors {
            top: parent.top
            left: parent.left
            right: sideBar.left
        }
        height: 40

        RowLayout {
            anchors.fill: parent

            Button {
                text: "Quit"
                Layout.alignment: Qt.AlignLeft
                Layout.preferredWidth: width

                onClicked: {
                    W.closeMap();
                    root.stack.pop();
                }
            }
            Button {
                text: "Open"
                Layout.alignment: Qt.AlignLeft
                Layout.preferredWidth: width

                onClicked: {
                    fileDialog.open();
                }
            }
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
                    map.setEditMode(Map.EditableMap.SelectMode);
                }
            }

            Button {
                text: "Create Map Nodes"
                Layout.alignment: Qt.AlignRight
                Layout.preferredWidth: width

                onClicked: {
                    editModes.currentIndex = 1;
                    map.setEditMode(Map.EditableMap.CreateMapNodeMode);
                }
            }

            Button {
                text: "Create Settlements"
                Layout.alignment: Qt.AlignRight
                Layout.preferredWidth: width

                onClicked: {
                    editModes.currentIndex = 2;
                    map.setEditMode(Map.EditableMap.CreateSettlementMode);
                }
            }

            Button {
                text: "Create Units"
                Layout.alignment: Qt.AlignRight
                Layout.preferredWidth: width

                onClicked: {
                    editModes.currentIndex = 3;
                    map.setEditMode(Map.EditableMap.CreateUnitMode);
                }
            }

            Button {
                text: "Edit Map Nodes"
                Layout.alignment: Qt.AlignRight
                Layout.preferredWidth: width

                onClicked: {
                    editModes.currentIndex = 4;
                    map.setEditMode(Map.EditableMap.EditMapNodeMode);
                }
            }

            Button {
                text: "Edit Settlement"
                Layout.alignment: Qt.AlignRight
                Layout.preferredWidth: width

                onClicked: {
                    editModes.currentIndex = 5;
                    map.setEditMode(Map.EditableMap.EditSettlementMode);
                }
            }

            Button {
                text: "Edit Unit"
                Layout.alignment: Qt.AlignRight
                Layout.preferredWidth: width

                onClicked: {
                    editModes.currentIndex = 6;
                    map.setEditMode(Map.EditableMap.EditUnitMode);
                }
            }
        }
    }

    Rectangle {
        id: sideBar
        anchors {
            top: parent.top
            right: parent.right
            bottom: parent.bottom
        }

        width: 256

        MiniMap {
            id: miniMap

            map: W.map

            anchors {
                left: parent.left
                top: parent.top
                right: parent.right
            }
            height: 288

            Component.onCompleted: {
                map.windowChanged.connect(miniMap.onMapWindowChanged);
            }

            function onMapWindowChanged(mapWindow) {
                miniMap.window = mapWindow;
            }
        }

        Rectangle {
            id: mapEditorControls

            signal terrainTypeSelected(string objectName)
            signal unitTypeSelected(string objectName)
            signal settlementTypeSelected(string objectName)

            onTerrainTypeSelected: map.setTerrainType(objectName)
            onSettlementTypeSelected: map.setSettlementType(objectName)
            onUnitTypeSelected: map.setUnitType(objectName)

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

                        function onSelectMapItems(mapNode, settlement, unit) {
                            mapItemsInfo.mapNode = mapNode;
                            mapItemsInfo.settlement = settlement;
                            mapItemsInfo.unit = unit;
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

                Tab {
                    id: editUnit
                    title: "Edit Unit"

                    UnitEdit {
                        id: unitEdit
                        anchors.fill: parent

                        function onEditUnit(unit) {
                            unitEdit.unit = unit;
                        }

                        Component.onCompleted: {
                            map.editUnit.connect(unitEdit.onEditUnit);
                        }
                    }
                }
            }
        }
    }

    EditableMap {
        id: map

        map: W.map

        anchors {
            left: parent.left
            top: statusBar.bottom
            right: sideBar.left
            bottom: parent.bottom
        }

        Component.onCompleted: {
            miniMap.windowPosChanged.connect(map.onMiniMapWindowPosChanged);
        }

        function onMiniMapWindowPosChanged(miniMapWindowPos) {
            map.windowPos = miniMapWindowPos;
        }
    }
}
