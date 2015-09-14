import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Rectangle {
    id: unitEdit
    property var unit

    Image {
        id: picture
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
            if (unitEdit.unit) {
                var unitTypeName = unitEdit.unit.unitType.objectName;
                surface.prefix + surface.bigMap[unitTypeName];
            } else {
                "";
            }
        }
    }

    Rectangle {
        anchors {
            top: parent.top
            bottom: parent.bottom
            left: picture.right
            right: parent.right

            leftMargin: 5
            rightMargin: 5
        }

        Label {
            id: objectNameLabel
            height: 15
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
                topMargin: 10
            }
            color: "black"
            text: {
                var label = "Unit ";
                if (unitEdit.unit) {
                    label += unitEdit.unit.objectName;
                }
                label;
            }
        }

        TextEditGroup {
            id: displayNameEdit
            anchors {
                top: objectNameLabel.bottom
                left: parent.left
                right: parent.right
                topMargin: 10
            }

            label: "Name"
            value: {
                if (unitEdit.unit) {
                    unitEdit.unit.displayName;
                } else {
                    "";
                }
            }

            onValueEdited: {
                if (unitEdit.unit) {
                    unitEdit.unit.displayName = val;
                }
            }
        }

        ListEditGroup {
            id: unitTypeEdit
            anchors {
                top: displayNameEdit.bottom
                left: parent.left
                right: parent.right
                topMargin: 10
            }

            label: "Unit Type"
            model: W.map.world.unitTypes
            currentIndex: {
                if (unitEdit.unit) {
                    unitTypeEdit.find(unitEdit.unit.unitType.displayName);
                } else {
                    0;
                }
            }

            onCurrentItemChanged: {
                if (unitEdit.unit) {
                    console.log(index);
                    unitEdit.unit.unitType = W.map.world.unitTypes[index];
                }
            }
        }

        ListEditGroup {
            id: ownerEdit
            anchors {
                top: unitTypeEdit.bottom
                left: parent.left
                right: parent.right
                topMargin: 10
            }

            label: "Owner"
            model: W.map.allPlayers
            currentIndex: {
                if (unitEdit.unit) {
                    ownerEdit.find(unitEdit.unit.owner.displayName);
                } else {
                    0;
                }
            }

            onCurrentItemChanged: {
                if (unitEdit.unit) {
                    unitEdit.unit.owner = W.map.allPlayers[index];
                }
            }
        }

        TextEditGroup {
            id: hpEdit
            anchors {
                top: ownerEdit.bottom
                left: parent.left
                right: parent.right
                topMargin: 10
            }

            label: {
                var maxPoints = 0
                if (unitEdit.unit) {
                    maxPoints = unitEdit.unit.unitType.hitPoints;
                }
                "HP (/" + maxPoints + ")";
            }
            value: {
                if (unitEdit.unit) {
                    unitEdit.unit.hitPoints;
                } else {
                    0;
                }
            }

            onValueEdited: {
                if (unitEdit.unit) {
                    unitEdit.unit.hitPoints = val;
                }
            }
        }

        TextEditGroup {
            id: mpEdit
            anchors {
                top: hpEdit.bottom
                left: parent.left
                right: parent.right
                topMargin: 10
            }

            label: {
                var maxPoints = 0
                if (unitEdit.unit) {
                    maxPoints = unitEdit.unit.unitType.unitClass.movementPoints;
                }
                "MP (/" + maxPoints + ")";
            }
            value: {
                if (unitEdit.unit) {
                    unitEdit.unit.movementPoints;
                } else {
                    0;
                }
            }

            onValueEdited: {
                if (unitEdit.unit) {
                    unitEdit.unit.movemenetPoints = val;
                }
            }
        }

        TextEditGroup {
            id: xpEdit
            anchors {
                top: mpEdit.bottom
                left: parent.left
                right: parent.right
                topMargin: 10
            }

            label: "XP"
            value: {
                if (unitEdit.unit) {
                    unitEdit.unit.experiencePoints;
                } else {
                    0;
                }
            }

            onValueEdited: {
                if (unitEdit.unit) {
                    unitEdit.unit.experiencePoints = val;
                }
            }
        }
    }
}
