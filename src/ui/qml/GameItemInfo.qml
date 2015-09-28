import QtQuick 2.2
import QtQuick.Layouts 1.1

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

        Text {
            id: unitName
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }

            text: {
                var n = root.mapNode;
                if (n && n.unit) {
                    var u = n.unit.qobj;
                    var text = "";

                    if (u.displayName != "")
                        text += u.displayName + " ";
                    text += u.unitType.displayName;
                    text += " [" + u.unitType.unitClass.displayName + "]";
                    text += " - " + u.owner.displayName;
                    text;
                } else {
                    "";
                }
            }
        }

        Rectangle {
            anchors {
                top: unitName.bottom
                left: parent.left
                right: parent.right
            }
            height: 100

            Rectangle {
                id: unitImage

                width: 100
                anchors {
                    top: parent.top
                    bottom: parent.bottom
                    left: parent.left
                }

                Image {
                    width: {
                        if (root.mapNode && root.mapNode.unit) {
                            var tileSize = root.world.surface.tileSize;
                            if (tileSize.width < tileSize.height) {
                                parent.height * tileSize.height/tileSize.width;
                            } else {
                                parent.width;
                            }
                        } else {
                            0;
                        }
                    }
                    height: {
                        if (root.mapNode && root.mapNode.unit) {
                            var tileSize = root.world.surface.tileSize;
                            if (tileSize.height > tileSize.width) {
                                parent.width * tileSize.width/tileSize.height;
                            } else {
                                parent.height;
                            }
                        } else {
                            0;
                        }
                    }
                    anchors {
                        top: parent.top
                        left: parent.left
                    }

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
            }

            Rectangle {
                anchors {
                    top: parent.top
                    bottom: parent.bottom
                    left: unitImage.right
                    right: parent.right
                    margins: 5
                }

                Column {
                    anchors.fill: parent
                    spacing: 5

                    RowLayout {
                        spacing: 5
                        height: 24
                        width: parent.width

                        Text {
                            Layout.preferredWidth: 25
                            anchors.verticalCenter: parent.verticalCenter
                            text: "HP"
                        }

                        PointBar {
                            Layout.fillWidth: true
                            height: parent.height * 0.8

                            points: {
                                var n = root.mapNode;
                                if (n && n.unit) n.unit.qobj.hitPoints;
                                else 0;
                            }
                            maxPoints: {
                                var n = root.mapNode;
                                if (n && n.unit) n.unit.qobj.unitType.hitPoints;
                                else 0;
                            }
                            color: {
                                if (maxPoints) {
                                    var proportion = points/maxPoints;
                                    if (points < 0.33) "red";
                                    else if (points > 0.33 && points < 0.66) "yellow";
                                    else "green";
                                } else {
                                    "";
                                }
                            }
                        }
                    }

                    RowLayout {
                        spacing: 5
                        height: 24
                        width: parent.width

                        Text {
                            Layout.preferredWidth: 25
                            anchors.verticalCenter: parent.verticalCenter
                            text: "MP"
                        }

                        PointBar {
                            Layout.fillWidth: true
                            height: parent.height * 0.8

                            points: {
                                var n = root.mapNode;
                                if (n && n.unit) n.unit.qobj.movementPoints;
                                else 0;
                            }
                            maxPoints: {
                                var n = root.mapNode;
                                if (n && n.unit) {
                                    var u = n.unit.qobj;
                                    u.unitType.unitClass.movementPoints;
                                } else {
                                    0;
                                }
                            }
                            color: "blue";
                        }
                    }

                    RowLayout {
                        spacing: 5
                        height: 24
                        width: parent.width

                        Text {
                            Layout.preferredWidth: 25
                            anchors.verticalCenter: parent.verticalCenter
                            text: "XP"
                        }

                        PointBar {
                            Layout.fillWidth: true
                            height: parent.height * 0.8

                            points: {
                                var n = root.mapNode;
                                if (n && n.unit) n.unit.qobj.experiencePoints;
                                else 0;
                            }
                            maxPoints: {
                                var n = root.mapNode;
                                if (n && n.unit) {
                                    //n.unit.qobj.experiencePoints;
                                    100;
                                } else {
                                    0;
                                }
                            }
                            color: "purple";
                        }
                    }
                }
            }
        }
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
