import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

Rectangle {
    id: root

    property var mapNode
    property var settlement
    property var unit

    Column {
        anchors.fill: parent

        Rectangle {
            id: turnInfo

            height: 24
            anchors {
                left: parent.left
                right: parent.right
            }

            Text {
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: parent.left
                }

                text: "Turn " + W.game.turn
            }
        }

        Rectangle {
            id: mapNodeInfo

            height: 24
            anchors {
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
                        surface.imagePaths[terrainTypeName];
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
            visible: {
                if (root.settlement) true
                else false
            }

            height: 24
            anchors {
                left: parent.left
                right: parent.right
            }

            Image {
                id: settlementImage
                height: 24
                width: 24
                source: {
                    var s = root.settlement;
                    var path = "";
                    if (s) {
                        var surface = W.world.surface;
                        var imageName = s.type.objectName;
                        path = surface.imagePaths[imageName];
                    }
                    path;
                }
                fillMode: Image.PreserveAspectFit

                anchors.left: parent.left
            }

            Text {
                anchors {
                    left: settlementImage.right
                    leftMargin: 2
                    verticalCenter: parent.verticalCenter
                }

                text: {
                    var s = root.settlement
                    var text = "";
                    if (s) {
                        var dn = s.displayName;
                        if (dn != "")
                            text += dn + " ";

                        text += s.type.displayName;
                        if (s.owner)
                            text += " - " + s.owner.displayName;
                    }
                    text;
                }
            }
        }

        Rectangle {
            visible: {
                if (root.unit) true
                else false
            }

            height: 148

            anchors {
                left: parent.left
                right: parent.right
            }

            Column {
                anchors.fill: parent

                Text {
                    height: 24
                    anchors {
                        left: parent.left
                        right: parent.right
                    }

                    text: {
                        var text = "";
                        var u = root.unit;

                        if (u) {
                            if (u.displayName != "")
                                text += u.displayName + " ";
                            text += u.type.displayName;
                            text += " [" + u.type.klass.displayName + "]";
                            if (u.owner)
                                text += " - " + u.owner.displayName;
                        }
                        text;
                    }
                }

                Text {
                    height: 24
                    anchors {
                        left: parent.left
                        right: parent.right
                    }

                    text: {
                        var text = "";
                        var u = root.unit;

                        if (u) {
                            text += "Level " + u.type.level.index;
                            text += " (" + u.type.level.displayName + ")";
                            text += " " + u.rankName;
                        }
                        text;
                    }
                }

                Rectangle {
                    height: 100
                    anchors {
                        left: parent.left
                        right: parent.right
                    }

                    Image {
                        id: unitImage

                        width: 100
                        height: 100
                        source: {
                            var path = "";
                            if (root.unit) {
                                var surface = W.world.surface;
                                var imageName = root.unit.type.objectName;
                                path = surface.imagePaths[imageName];
                            }
                            path;
                        }

                        fillMode: Image.PreserveAspectFit

                        anchors {
                            top: parent.top
                            bottom: parent.bottom
                            left: parent.left
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

                                    points: if (root.unit) root.unit.hitPoints;
                                    maxPoints: if (root.unit) root.unit.type.hitPoints
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

                                    points: if (root.unit) root.unit.movementPoints
                                    maxPoints: if (root.unit) root.unit.type.klass.movementPoints
                                    color: "blue"
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

                                    points: if (root.unit) root.unit.experiencePoints
                                    maxPoints: if (root.unit) root.unit.type.level.experiencePoints
                                    color: "purple"
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
