import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import Warmonger 1.0

Rectangle {
    id: root

    property var map
    readonly property var stack: Stack.view

     Stack.onStatusChanged: {
        if (Stack.status === Stack.Active) {
            map.mode = GameMap.MoveMode;
        }
    }

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
                if (root.focusedMapNode) {
                    var surface = W.world.surface;
                    var terrainTypeName = root.focusedMapNode.terrainType.objectName;
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
                if (root.focusedMapNode) {
                    var text = root.focusedMapNode.displayName + " ";
                    text += root.focusedMapNode.terrainType.displayName;
                    text;
                } else {
                    "";
                }
            }
        }
    }

    WButton {
        id: settlementInfo
        visible: {
            if (W.hasSettlement(root.map.focusedMapNode)) true
            else false;
        }

        readonly property var settlement: W.getSettlementOn(root.map.focusedMapNode)

        height: 24
        anchors {
            top: mapNodeInfo.bottom
            left: parent.left
            right: parent.right
        }

        Image {
            id: settlementImage
            height: 24
            width: 24
            source: {
                var s = settlementInfo.settlement;
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
                var s = settlementInfo.settlement
                var text = "";
                if (s) {
                    var dn = s.displayName;
                    if (dn != "")
                        text += dn + " ";

                    text += s.type.displayName;
                    text += " - " + s.owner.displayName;
                }
                text;
            }
        }

        onClicked: {
            root.stack.push({
                "item": Qt.createComponent("RecruitUnits.qml"),
                "properties": {
                    "map": root.map
                }
            });
        }
    }

    Rectangle {
        id: unitInfo
        visible: {
            if (W.hasUnit(root.map.focusedMapNode)) true
            else false;
        }

        readonly property var unit: W.getUnitOn(root.map.focusedMapNode)

        anchors {
            top: settlementInfo.bottom
            bottom: parent.bottom
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
                var text = "";
                var u = unitInfo.unit;

                if (u) {
                    if (u.displayName != "")
                        text += u.displayName + " ";
                    text += u.type.displayName;
                    text += " [" + u.type.klass.displayName + "]";
                    text += " - " + u.owner.displayName;
                }
                text;
            }
        }

        Text {
            id: unitRank
            anchors {
                top: unitName.bottom
                left: parent.left
                right: parent.right
            }

            text: {
                var text = "";
                var u = unitInfo.unit;

                if (u) {
                    text += "Level " + u.type.level.index;
                    text += " (" + u.type.level.displayName + ")";
                    text += " " + u.rankName;
                }
                text;
            }
        }

        Rectangle {
            anchors {
                top: unitRank.bottom
                left: parent.left
                right: parent.right
            }
            height: 100

            Image {
                id: unitImage

                width: 100
                height: 100
                source: {
                    var path = "";
                    if (unitInfo.unit) {
                        var surface = W.world.surface;
                        var imageName = unitInfo.unit.type.objectName;
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

                            points: if (unitInfo.unit) unitInfo.unit.hitPoints;
                            maxPoints: if (unitInfo.unit) unitInfo.unit.type.hitPoints
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

                            points: if (unitInfo.unit) unitInfo.unit.movementPoints
                            maxPoints: if (unitInfo.unit) unitInfo.unit.type.klass.movementPoints
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

                            points: if (unitInfo.unit) unitInfo.unit.experiencePoints
                            maxPoints: if (unitInfo.unit) unitInfo.unit.type.level.experiencePoints
                            color: "purple"
                        }
                    }
                }
            }
        }
    }
}
