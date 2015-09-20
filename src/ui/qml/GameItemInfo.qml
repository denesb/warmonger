import QtQuick 2.2

Rectangle {
    id: root

    property var world
    property var mapNode

    Column {
        Rectangle {
            height: 24
            anchors {
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
    }
}
