import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Rectangle {
    id: mapItemsInfo
    property var mapNode
    property var settlement

    Rectangle {
        id: mapNodeInfo
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

        height: 94

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
                    "images:" + surface.gameMap[terrainTypeName];
                } else {
                    "images:" + surface.gameMap["undefined"];
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
                id: mapNodeTerrainType
                anchors {
                    top: mapNodeObjectName.bottom
                    left: parent.left
                    right: parent.right
                    topMargin: 10
                }
                color: "black"
                text: {
                    if (mapItemsInfo.mapNode) {
                        mapItemsInfo.mapNode.terrainType.displayName;
                    } else {
                        "";
                    }
                }
            }

            Label {
                id: mapNodeDisplayName
                anchors {
                    top: mapNodeTerrainType.bottom
                    left: parent.left
                    right: parent.right
                    topMargin: 10
                }
                color: "black"
                text: {
                    if (mapItemsInfo.mapNode) {
                        mapItemsInfo.mapNode.displayName;
                    } else {
                        "";
                    }
                }
            }
        }
    }

    Rectangle {
        id: settlementInfo
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
                    "images:" + surface.gameMap[settlementTypeName];
                } else {
                    "images:" + surface.gameMap["undefined"];
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
                id: settlementSettlementType
                anchors {
                    top: settlementObjectName.bottom
                    left: parent.left
                    right: parent.right
                    topMargin: 10
                }
                color: "black"
                text: {
                    if (mapItemsInfo.settlement) {
                        mapItemsInfo.settlement.settlementType.displayName;
                    } else {
                        "";
                    }
                }
            }

            Label {
                id: settlementDisplayName
                anchors {
                    top: settlementSettlementType.bottom
                    left: parent.left
                    right: parent.right
                    topMargin: 10
                }
                color: "black"
                text: {
                    if (mapItemsInfo.settlement) {
                        mapItemsInfo.settlement.displayName;
                    } else {
                        "";
                    }
                }
            }
        }
    }

}
