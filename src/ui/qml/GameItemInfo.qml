import QtQuick 2.2

Rectangle {
    id: root

    property var world
    property var mapNode

    Rectangle {
        id: mapNodeInfo

        height: 24
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

        Image {
            id: mapNodeImage
            height: 20
            width: {
                var tileSize = root.world.surface.tileSize;
                height * tileSize.width/tileSize.height;
            }
            anchors {
                left: parent.left
                leftMargin: 2
                verticalCenter: parent.verticalCenter
            }

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
            anchors {
                left: mapNodeImage.right
                right: parent.right
                leftMargin: 2
                verticalCenter: parent.verticalCenter
            }

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
    }

    Rectangle {
        id: settlementInfo

        height: 24
        anchors {
            top: mapNodeInfo.bottom
            left: parent.left
            right: parent.right
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
            anchors {
                left: parent.left
                verticalCenter: parent.verticalCenter
            }

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
            anchors {
                left: settlementImage.right
                leftMargin: 2
                verticalCenter: parent.verticalCenter
            }

            text: {
                var n = root.mapNode;
                if (n && n.settlement) {
                    var s = n.settlement.qobj;
                    var text = s.displayName + " ";
                    text += s.settlementType.displayName;
                    text += " - " + s.owner.displayName;
                    text;
                } else {
                    "";
                }
            }
        }
    }

    Rectangle {
        id: unitInfo

        anchors {
            top: settlementInfo.bottom
            bottom: turnInfo.top
            left: parent.left
            right: parent.right
        }

        color: "gold"
    }

    Rectangle {
        id: turnInfo

        height: 24
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }
        color: "green"
    }
}
