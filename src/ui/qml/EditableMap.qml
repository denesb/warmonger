import QtQuick 2.2

import "js/Map.js" as Map

Rectangle {
    id: root

    property var map;
    property var windowPos;

    signal windowChanged(var window)
    signal selectMapItems(var mapNode, var settlement, var unit)
    signal editMapNode(var mapNode)
    signal editSettlement(var settlement)
    signal editUnit(var unit)

    function onMiniMapWindowPosChanged(windowPos) {
        if (jobj !== undefined) {
            jobj.onWindowPosChanged(windowPos);
        }
    }

    function setEditMode(editMode) {
        canvas.jobj.setEditMode(editMode);
    }

    function setTerrainType(terrainTypeObjectName) {
        canvas.jobj.setTerrainType(terrainTypeObjectName);
    }

    function setSettlementType(settlementTypeObjectName) {
        canvas.jobj.setSettlementType(settlementTypeObjectName);
    }

    function setUnitType(unitTypeObjectName) {
        canvas.jobj.setUnitType(unitTypeObjectName);
    }

    onWindowPosChanged: {
        if (canvas.jobj) {
            canvas.jobj.onWindowPosChanged(windowPos);
        }
    }

    border {
        width: 1
        color: "black"
    }

    Canvas {
        id: canvas

        property var jobj

        anchors.fill: parent

        onCanvasWindowChanged: {
            root.windowChanged(canvasWindow);
        }
        onPaint: {
            if (jobj) {
                jobj.onPaint(region);
            }
        }
        Component.onCompleted: {
            Map.W = W;
            root.mapChanged.connect(onMapChanged);
            onMapChanged();
        }

        function onMapChanged() {
            if (root.map) {
                jobj = new Map.EditableMap(root.map, canvas, mouseArea);
                jobj.onSelectMapItems = root.selectMapItems;
                jobj.onEditMapNode = root.editMapNode;
                jobj.onEditSettlement = root.editSettlement;
                jobj.onEditUnit = root.editUnit;
            } else {
                jobj = undefined;
            }
        }

        MouseArea {
            id: mouseArea

            anchors.fill: parent
            hoverEnabled: true
            acceptedButtons: Qt.LeftButton

            onPressed: {
                if (canvas.jobj) {
                    canvas.jobj.onPressed(mouse);
                }
            }
            onReleased: {
                if (canvas.jobj) {
                    canvas.jobj.onReleased(mouse);
                }
            }
            onPositionChanged: {
                if (canvas.jobj) {
                    canvas.jobj.onPositionChanged(mouse);
                }
            }
        }
    }
}
