import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Rectangle {
    id: unitEdit
    property var unit

    onUnitChanged: {
        var unitTypeIndex = -1;
        if (unitEdit.unit) {
            unitTypeIndex = unitTypeEdit.find(unitEdit.unit.unitType.displayName);
        }
        unitTypeEdit.currentIndex = unitTypeIndex;
    }

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
                "images:" + surface.bigMap[unitTypeName];
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

        Label {
            id: displayNameLabel
            anchors {
                top: objectNameLabel.bottom
                left: parent.left
                right: parent.right
                topMargin: 10
            }
            color: "black"
            text: "Display Name:"
        }

        TextField {
            id: displayNameEdit
            height: 25
            anchors {
                top: displayNameLabel.bottom
                left: parent.left
                right: parent.right
            }
            style: TextFieldStyle {
                textColor: "black"
                background: Rectangle {
                    radius: 5
                    border.color: "#333"
                    border.width: 1
                }
            }

            text: {
                if (unitEdit.unit) {
                    unitEdit.unit.displayName
                } else {
                    ''
                }
            }

            onEditingFinished: {
                if (unitEdit.unit) {
                    unitEdit.unit.displayName = displayNameEdit.text;
                }
            }
        }

        Label {
            id: unitTypeLabel
            anchors {
                top: displayNameEdit.bottom
                left: parent.left
                right: parent.right
                topMargin: 10
            }
            color: "black"
            text: "Unit Type:"
        }

        ComboBox {
            id: unitTypeEdit
            height: 25
            anchors {
                top: unitTypeLabel.bottom
                left: parent.left
                right: parent.right
            }
            style: ComboBoxStyle {
                background: Rectangle {
                    radius: 5
                    border.color: "#333"
                    border.width: 1
                }
            }

            model: W.map.world.unitTypes
            textRole: "displayName"

            onActivated: {
                if (unitEdit.unit) {
                    var unitTypes = W.map.world.unitTypes;
                    var unitType = unitTypes[index];

                    unitEdit.unit.unitType = unitType;
                }
            }
        }

        OwnerEdit {
            id: ownerEdit
            anchors {
                top: unitTypeEdit.bottom
                left: parent.left
                right: parent.right
                topMargin: 10
            }

            entityName: "Unit"
            ownersModel: W.map.allPlayers

            ownerIndex: {
                if (unitEdit.unit) {
                    ownerEdit.find(unitEdit.unit.owner.displayName);
                } else {
                    0;
                }
            }

            onOwnerEdited: {
                if (unitEdit.unit) {
                    unitEdit.unit.owner = W.map.allPlayers[index];
                }
            }
        }

        PointsEdit {
            id: hpEdit
            anchors {
                top: ownerEdit.bottom
                left: parent.left
                right: parent.right
                topMargin: 10
            }

            pointsName: "HP"
            maxPoints: {
                if (unitEdit.unit) {
                    unitEdit.unit.unitType.hitPoints;
                } else {
                    0;
                }
            }
            points: {
                if (unitEdit.unit) {
                    unitEdit.unit.hitPoints;
                } else {
                    0;
                }
            }

            onPointsEdited: {
                if (unitEdit.unit) {
                    unitEdit.unit.hitPoints = val
                } else {
                    0;
                }
            }
        }

        PointsEdit {
            id: mpEdit
            anchors {
                top: hpEdit.bottom
                left: parent.left
                right: parent.right
                topMargin: 10
            }

            pointsName: "MP"
            maxPoints: {
                if (unitEdit.unit) {
                    unitEdit.unit.unitType.unitClass.movementPoints;
                } else {
                    0;
                }
            }
            points: {
                if (unitEdit.unit) {
                    unitEdit.unit.movementPoints;
                } else {
                    0;
                }
            }

            onPointsEdited: {
                if (unitEdit.unit) {
                    unitEdit.unit.movemenetPoints = val
                } else {
                    0;
                }
            }
        }
    }
}
