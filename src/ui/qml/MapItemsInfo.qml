import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Rectangle {
    id: mapItemsInfo
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
                var tileSize = W.map.world.surface.tileSize;
                64 * tileSize.height/tileSize.width;
            }
            anchors {
                top: parent.top
                left: parent.left
                margins: 2
            }
        
            source: {
                var surface = W.map.world.surface;
                if (mapItemsInfo.mapNode) {
                    var terrainTypeName = mapItemsInfo.mapNode.terrainType.objectName;
                    "images:" + surface.bigMap[terrainTypeName];
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
                    if (mapItemsInfo.mapNode) {
                        label += mapItemsInfo.mapNode.objectName;
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
                    if (mapItemsInfo.mapNode) {
                        label += mapItemsInfo.mapNode.displayName;
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
                    if (mapItemsInfo.mapNode) {
                        label += mapItemsInfo.mapNode.terrainType.displayName;
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
                var tileSize = W.map.world.surface.tileSize;
                64 * tileSize.height/tileSize.width;
            }
            anchors {
                top: parent.top
                left: parent.left
                margins: 2
            }
        
            source: {
                var surface = W.map.world.surface;
                if (mapItemsInfo.settlement) {
                    var settlementTypeName = mapItemsInfo.settlement.settlementType.objectName;
                    "images:" + surface.bigMap[settlementTypeName];
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
                    if (mapItemsInfo.settlement) {
                        label += mapItemsInfo.settlement.objectName;
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
                    if (mapItemsInfo.settlement) {
                        label += mapItemsInfo.settlement.displayName;
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
                    if (mapItemsInfo.settlement) {
                        label += mapItemsInfo.settlement.settlementType.displayName;
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
                    if (mapItemsInfo.settlement) {
                        label += mapItemsInfo.settlement.owner.displayName;
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
                var tileSize = W.map.world.surface.tileSize;
                64 * tileSize.height/tileSize.width;
            }
            anchors {
                top: parent.top
                left: parent.left
                margins: 2
            }

            source: {
                var surface = W.map.world.surface;
                if (mapItemsInfo.unit) {
                    var unitTypeName = mapItemsInfo.unit.unitType.objectName;
                    "images:" + surface.bigMap[unitTypeName];
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
                    if (mapItemsInfo.unit) {
                        label += mapItemsInfo.unit.objectName;
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
                    if (mapItemsInfo.unit) {
                        label += mapItemsInfo.unit.displayName;
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
                    if (mapItemsInfo.unit) {
                        label += mapItemsInfo.unit.unitType.displayName;
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
                    if (mapItemsInfo.unit) {
                        label += mapItemsInfo.unit.owner.displayName;
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
                    if (mapItemsInfo.unit) {
                        label += mapItemsInfo.unit.hitPoints;
                        label += "/";
                        label += mapItemsInfo.unit.unitType.hitPoints;
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
                    if (mapItemsInfo.unit) {
                        label += mapItemsInfo.unit.movementPoints;
                        label += "/";
                        label += mapItemsInfo.unit.unitType.unitClass.movementPoints;
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
                    if (mapItemsInfo.unit) {
                        label += mapItemsInfo.unit.experiencePoints;
                    }
                    label;
                }
            }
        }
    }
}
