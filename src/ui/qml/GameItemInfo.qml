import QtQuick 2.2
import QtQuick.Layouts 1.1

Rectangle {
    id: root

    property var world
    property var mapNode
    property var settlement
    property var unit

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
                var tileSize = W.world.surface.tileSize;
                height * tileSize.width/tileSize.height;
            }
            anchors {
                left: parent.left
                leftMargin: 2
                verticalCenter: parent.verticalCenter
            }

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
            anchors {
                left: mapNodeImage.right
                right: parent.right
                leftMargin: 2
                verticalCenter: parent.verticalCenter
            }

            text: {
                if (root.mapNode) {
                    var text = root.mapNode.displayName + " ";
                    text += root.mapNode.terrainType.displayName;
                    text;
                } else {
                    "";
                }
            }
        }
    }

    Rectangle {
        id: settlementInfo
        visible: {
            if (root.settlement) true;
            else false;
        }

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
                if (root.settlement) {
                    var tileSize = W.world.surface.tileSize;
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
                if (root.settlement) {
                    var surface = W.world.surface;
                    var tn = root.settlement.type.objectName;
                    surface.prefix + surface.imagePaths[tn];
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
                if (root.settlement) {
                    var text = "";

                    var dn = root.settlement.displayName;
                    if (dn != "")
                        text += dn + " ";

                    text += root.settlement.type.displayName;
                    text += " - " + root.settlement.owner.displayName;
                    text;
                } else {
                    "";
                }
            }
        }
    }

    Rectangle {
        id: unitInfo
        visible: {
            if (root.unit) true;
            else false;
        }

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
                if (root.unit) {
                    var u = root.unit;
                    var text = "";

                    if (u.displayName != "")
                        text += u.displayName + " ";
                    text += u.type.displayName;
                    text += " [" + u.type.klass.displayName + "]";
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
                        if (root.unit) {
                            var tileSize = W.world.surface.tileSize;
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
                        if (root.unit) {
                            var tileSize = W.world.surface.tileSize;
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
                        if (root.unit) {
                            var surface = W.world.surface;
                            var typeName = root.unit.type.objectName;
                            surface.prefix + surface.imagePaths[typeName];
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
                                if (root.unit) root.unit.hitPoints;
                                else 0;
                            }
                            maxPoints: {
                                if (root.unit) root.unit.type.hitPoints;
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
                                if (root.unit) root.unit.movementPoints;
                                else 0;
                            }
                            maxPoints: {
                                if (root.unit) {
                                    root.unit.type.klass.movementPoints;
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
                                if (root.unit) root.unit.experiencePoints;
                                else 0;
                            }
                            maxPoints: {
                                if (root.unit) {
                                    root.unit.type.level.experiencePoints;
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
