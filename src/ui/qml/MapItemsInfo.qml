import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Rectangle {
    id: root
    property var mapNode
    property var settlement
    property var unit

    Rectangle {
        id: mapNodeInfo
        height: 90
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

        Image {
            id: mapNodeImage
            width: 64
            height: {
                if (W.map !== null) {
                    var tileSize = W.map.world.surface.tileSize;
                    64 * tileSize.height/tileSize.width;
                } else {
                    64;
                }
            }
            anchors {
                top: parent.top
                left: parent.left
                margins: 2
            }
        
            source: {
                if (W.map !== undefined && root.mapNode !== undefined) {
                    var surface = W.map.world.surface;
                    var terrainTypeName = root.mapNode.terrainType.objectName;
                    surface.bigMap[terrainTypeName];
                } else {
                    "";
                }
            }
        }

        Rectangle {
            anchors {
                top: parent.top
                bottom: parent.bottom
                left: mapNodeImage.right
                right: parent.right

                leftMargin: 5
                rightMargin: 5
            }

            Label {
                id: mapNodeObjectName
                anchors {
                    top: parent.top
                    left: parent.left
                    right: parent.right
                    topMargin: 10
                }
                color: "black"
                text: {
                    var label = "Map Node ";
                    if (root.mapNode) {
                        label += root.mapNode.objectName;
                    }
                    label;
                }
            }

            Label {
                id: mapNodeDisplayName
                anchors {
                    top: mapNodeObjectName.bottom
                    left: parent.left
                    right: parent.right
                    topMargin: 10
                }
                color: "black"
                text: {
                    var label = "Name: ";
                    if (root.mapNode) {
                        label += root.mapNode.displayName;
                    }
                    label;
                }
            }

            Label {
                id: mapNodeTerrainType
                anchors {
                    top: mapNodeDisplayName.bottom
                    left: parent.left
                    right: parent.right
                    topMargin: 10
                }
                color: "black"
                text: {
                    var label = "Terrain Type: ";
                    if (root.mapNode) {
                        label += root.mapNode.terrainType.displayName;
                    }
                    label;
                }
            }
        }
    }

    Rectangle {
        id: settlementInfo
        height: 110
        anchors {
            top: mapNodeInfo.bottom
            left: parent.left
            right: parent.right
        }

        Image {
            id: settlementImage
            width: 64
            height: {
                if (W.map) {
                    var tileSize = W.map.world.surface.tileSize;
                    64 * tileSize.height/tileSize.width;
                } else {
                    64;
                }
            }
            anchors {
                top: parent.top
                left: parent.left
                margins: 2
            }
        
            source: {
                if (W.map && root.settlement) {
                    var surface = W.map.world.surface;
                    var settlementTypeName = root.settlement.settlementType.objectName;
                    surface.bigMap[settlementTypeName];
                } else {
                    "";
                }
            }
        }

        Rectangle {
            anchors {
                top: parent.top
                bottom: parent.bottom
                left: settlementImage.right
                right: parent.right

                leftMargin: 5
                rightMargin: 5
            }

            Label {
                id: settlementObjectName
                anchors {
                    top: parent.top
                    left: parent.left
                    right: parent.right
                    topMargin: 10
                }
                color: "black"
                text: {
                    var label = "Settlement ";
                    if (root.settlement) {
                        label += root.settlement.objectName;
                    }
                    label;
                }
            }

            Label {
                id: settlementDisplayName
                anchors {
                    top: settlementObjectName.bottom
                    left: parent.left
                    right: parent.right
                    topMargin: 10
                }
                color: "black"
                text: {
                    var label = "Name: ";
                    if (root.settlement) {
                        label += root.settlement.displayName;
                    }
                    label;
                }
            }

            Label {
                id: settlementSettlementType
                anchors {
                    top: settlementDisplayName.bottom
                    left: parent.left
                    right: parent.right
                    topMargin: 10
                }
                color: "black"
                text: {
                    var label = "Settlement Type: ";
                    if (root.settlement) {
                        label += root.settlement.settlementType.displayName;
                    }
                    label;
                }
            }

            Label {
                id: settlementOwner
                anchors {
                    top: settlementSettlementType.bottom
                    left: parent.left
                    right: parent.right
                    topMargin: 10
                }
                color: "black"
                text: {
                    var label = "Owner: ";
                    if (root.settlement) {
                        label += root.settlement.owner.displayName;
                    }
                    label;
                }
            }
        }
    }

    Rectangle {
        id: unitInfo
        height: 128
        anchors {
            top: settlementInfo.bottom
            left: parent.left
            right: parent.right
        }

        Image {
            id: unitImage
            width: 64
            height: {
                if (W.map) {
                    var tileSize = W.map.world.surface.tileSize;
                    64 * tileSize.height/tileSize.width;
                } else {
                    64;
                }
            }
            anchors {
                top: parent.top
                left: parent.left
                margins: 2
            }

            source: {
                if (W.map && root.unit) {
                    var surface = W.map.world.surface;
                    var unitTypeName = root.unit.unitType.objectName;
                    surface.bigMap[unitTypeName];
                } else {
                    "";
                }
            }
        }

        Rectangle {
            anchors {
                top: parent.top
                bottom: parent.bottom
                left: unitImage.right
                right: parent.right

                leftMargin: 5
                rightMargin: 5
            }

            Label {
                id: unitObjectName
                anchors {
                    top: parent.top
                    left: parent.left
                    right: parent.right
                    topMargin: 10
                }
                color: "black"
                text: {
                    var label = "Unit ";
                    if (root.unit) {
                        label += root.unit.objectName;
                    }
                    label;
                }
            }

            Label {
                id: unitDisplayName
                anchors {
                    top: unitObjectName.bottom
                    left: parent.left
                    right: parent.right
                    topMargin: 10
                }
                color: "black"
                text: {
                    var label = "Name: ";
                    if (root.unit) {
                        label += root.unit.displayName;
                    }
                    label;
                }
            }

            Label {
                id: unitUnitType
                anchors {
                    top: unitDisplayName.bottom
                    left: parent.left
                    right: parent.right
                    topMargin: 10
                }
                color: "black"
                text: {
                    var label = "Unit Type: ";
                    if (root.unit) {
                        label += root.unit.unitType.displayName;
                    }
                    label;
                }
            }

            Label {
                id: unitOwner
                anchors {
                    top: unitUnitType.bottom
                    left: parent.left
                    right: parent.right
                    topMargin: 10
                }
                color: "black"
                text: {
                    var label = "Owner: ";
                    if (root.unit) {
                        label += root.unit.owner.displayName;
                    }
                    label;
                }
            }

            Label {
                id: unitHP
                anchors {
                    top: unitOwner.bottom
                    left: parent.left
                    right: parent.right
                    topMargin: 10
                }
                color: "black"
                text: {
                    var label = "HP: ";
                    if (root.unit) {
                        label += root.unit.hitPoints;
                        label += "/";
                        label += root.unit.unitType.hitPoints;
                    }
                    label;
                }
            }

            Label {
                id: unitMP
                anchors {
                    top: unitHP.bottom
                    left: parent.left
                    right: parent.right
                    topMargin: 10
                }
                color: "black"
                text: {
                    var label = "MP: ";
                    if (root.unit) {
                        label += root.unit.movementPoints;
                        label += "/";
                        label += root.unit.unitType.unitClass.movementPoints;
                    }
                    label;
                }
            }

            Label {
                id: unitXP
                anchors {
                    top: unitMP.bottom
                    left: parent.left
                    right: parent.right
                    topMargin: 10
                }
                color: "black"
                text: {
                    var label = "XP: ";
                    if (root.unit) {
                        label += root.unit.experiencePoints;
                    }
                    label;
                }
            }
        }
    }
}
