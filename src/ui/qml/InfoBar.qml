import QtQuick 2.2

Rectangle {
    id: root

    property var mapNode
    property var settlement
    property var unit

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
                var tileSize = W.world.surface.tileSize;
                height * tileSize.width/tileSize.height;
            }
            anchors.verticalCenter: parent.verticalCenter

            source: {
                if (root.mapNode) {
                    var surface = W.world.surface;
                    var terrainTypeName = root.mapNode.terrainType.objectName;
                    surface.prefix + surface.imagePaths[terrainTypeName];
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
                    var text = "";
                    if (n.displayName != "")
                        text += n.displayName + " ";
                    text += n.terrainType.displayName;
                    text;
                } else {
                    "";
                }
            }
        }

        Rectangle {
            width: {
                if (root.settlement)
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
                if (root.settlement) {
                    var tileSize = W.world.surface.tileSize;
                    height * tileSize.width/tileSize.height;
                } else {
                    0;
                }
            }
            anchors.verticalCenter: parent.verticalCenter

            source: {
                if (root.settlement) {
                    var surface = W.world.surface;
                    var settlementTypeName = root.settlement.settlementType.objectName;
                    surface.prefix + surface.imagePaths[settlementTypeName];
                } else {
                    "";
                }
            }
        }

        Text {
            anchors.verticalCenter: parent.verticalCenter

            text: {
                if (root.settlement) {
                    var text = "";
                    if (root.settlement.displayName != "")
                        text += root.settlement.displayName + " ";
                    text += root.settlement.settlementType.displayName;
                    text;
                } else {
                    "";
                }
            }
        }

        Rectangle {
            width: {
                if (root.unit)
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
                if (root.unit) {
                    var tileSize = W.world.surface.tileSize;
                    height * tileSize.width/tileSize.height;
                } else {
                    0;
                }
            }
            anchors.verticalCenter: parent.verticalCenter

            source: {
                if (root.unit) {
                    var surface = W.world.surface;
                    var unitTypeName = root.unit.unitType.objectName;
                    surface.prefix + surface.imagePaths[unitTypeName];
                } else {
                    "";
                }
            }
        }

        Text {
            anchors.verticalCenter: parent.verticalCenter

            text: {
                if (root.unit) {
                    var text = "";

                    if (root.unit.displayName != "")
                        text += root.unit.displayName + " ";
                    text += root.unit.unitType.displayName;
                    text;
                } else {
                    "";
                }
            }
        }
    }
}
