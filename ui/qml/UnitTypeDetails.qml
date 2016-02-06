import QtQuick 2.2
import QtQuick.Layouts 1.1

Rectangle {
    id: root

    property var unitType;

    ColumnLayout {
        anchors.fill: parent

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 100

            Image {
                id: unitImage

                width: 100
                height: 100
                source: {
                    var ut = root.unitType;
                    var surface = W.world.surface;
                    surface.imagePaths[ut.objectName];
                }

                fillMode: Image.PreserveAspectFit

                anchors {
                    top: parent.top
                    bottom: parent.bottom
                    left: parent.left
                }
            }

            Text {
                anchors {
                    top: parent.top
                    bottom: parent.bottom
                    left: unitImage.right
                    right: parent.right
                }

                text: {
                    var ut = root.unitType;
                    var text = "";
                    text += ut.displayName;
                    text += " [" + ut.klass.displayName + "]";
                    text += "\n";
                    text += "Level " + ut.level.index;
                    text += " (" + ut.level.displayName + ")";
                    text;
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true

            GridLayout {
                anchors.fill: parent

                columns: 2

                Text {
                    text: "Hit points"
                }

                Text {
                    text: root.unitType.hitPoints
                }

                Text {
                    text: "Movement points"
                }

                Text {
                    text: root.unitType.klass.movementPoints
                }

                Text {
                    text: "Armor"
                }

                Text {
                    text: root.unitType.armor.displayName
                }

                Text {
                    Layout.alignment: Qt.AlignTop
                    text: "Weapons"
                }

                Text {
                    text: {
                        var text = "";
                        var weapons = root.unitType.weapons
                        for (var i = 0; i < weapons.length; i++) {
                            var weapon = weapons[i];
                            text += weapon.displayName;
                            text += "\n";
                        }
                        text;
                    }
                }

                Rectangle {
                    Layout.fillHeight: true
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
            }
        }
    }
}
