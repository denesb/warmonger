import QtQuick 2.2

Rectangle {
    id: root

    property var world
    property var mapNode

    Row {
        anchors {
            top: parent.top
            bottom: parent.bottom
            left: parent.left
            leftMargin: 4
            right: parent.right
            rightMargin: 4
        }
        spacing: 2

        Image {
            id: mapNodeImage
            height: 20
            width: {
                var tileSize = root.world.surface.tileSize;
                height * tileSize.width/tileSize.height;
            }
            anchors.verticalCenter: parent.verticalCenter

            source: {
                if (root.mapNode) {
                    var surface = root.world.surface;
                    var terrainTypeName = root.mapNode.qobj.terrainType.objectName;
                    surface.prefix + surface.bigMap[terrainTypeName];
                } else {
                    "";
                }
            }
        }

        Text {
            anchors.verticalCenter: parent.verticalCenter

            text: {
                if (root.mapNode) {
                    var n = root.mapNode;
                    var text = n.qobj.displayName + " ";
                    text += "(" + n.pos.x + "," + n.pos.y + ") ";
                    text += n.qobj.terrainType.displayName;
                    text;
                } else {
                    "";
                }
            }
        }

        Rectangle {
            width: {
                if (root.mapNode && root.mapNode.settlement)
                    1;
                else
                    0;
            }
            height: parent.height * 0.6
            anchors.verticalCenter: parent.verticalCenter

            color: "grey"
        }

        Image {
            id: settlementImage
            height: 20
            width: {
                if (root.mapNode && root.mapNode.settlement) {
                    var tileSize = root.world.surface.tileSize;
                    height * tileSize.width/tileSize.height;
                } else {
                    0;
                }
            }
            anchors.verticalCenter: parent.verticalCenter

            source: {
                var n = root.mapNode;
                if (n && n.settlement) {
                    var surface = root.world.surface;
                    var settlementTypeName = n.settlement.qobj.settlementType.objectName;
                    surface.prefix + surface.bigMap[settlementTypeName];
                } else {
                    "";
                }
            }
        }

        Text {
            anchors.verticalCenter: parent.verticalCenter

            text: {
                var n = root.mapNode;
                if (n && n.settlement) {
                    var text = n.settlement.qobj.displayName + " ";
                    text += n.settlement.qobj.settlementType.displayName;
                    text;
                } else {
                    "";
                }
            }
        }

        Rectangle {
            width: {
                if (root.mapNode && root.mapNode.unit)
                    1;
                else
                    0;
            }
            height: parent.height * 0.6
            anchors.verticalCenter: parent.verticalCenter

            color: "grey"
        }

        Image {
            id: unitImage
            height: 20
            width: {
                if (root.mapNode && root.mapNode.unit) {
                    var tileSize = root.world.surface.tileSize;
                    height * tileSize.width/tileSize.height;
                } else {
                    0;
                }
            }
            anchors.verticalCenter: parent.verticalCenter

            source: {
                var n = root.mapNode;
                if (n && n.unit) {
                    var surface = root.world.surface;
                    var unitTypeName = n.unit.qobj.unitType.objectName;
                    surface.prefix + surface.bigMap[unitTypeName];
                } else {
                    "";
                }
            }
        }

        Text {
            anchors.verticalCenter: parent.verticalCenter

            text: {
                var n = root.mapNode;
                if (n && n.unit) {
                    var text = n.unit.qobj.displayName + " ";
                    text += n.unit.qobj.unitType.displayName;
                    text;
                } else {
                    "";
                }
            }
        }
    }
}
