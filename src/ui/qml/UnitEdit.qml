import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Rectangle {
    id: unitEdit
    property var unit

    onUnitChanged: {
        var unitTypeIndex = -1;
        var ownerIndex = -1;
        if (unitEdit.unit) {
            unitTypeIndex = unitTypeEdit.find(unitEdit.unit.unitType.displayName);
            if (unitEdit.unit.owner) {
                ownerIndex = ownerEdit.find(unitEdit.unit.owner.displayName);
            }
        }
        unitTypeEdit.currentIndex = unitTypeIndex;
        ownerEdit.currentIndex = ownerIndex + 1;
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
                "images:" + surface.gameMap[unitTypeName];
            } else {
                "images:" + surface.gameMap["undefined"];
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

        Label {
            id: unitOwnerLabel
            anchors {
                top: unitTypeEdit.bottom
                left: parent.left
                right: parent.right
                topMargin: 10
            }
            color: "black"
            text: "Unit Owner:"
        }

        ComboBox {
            id: ownerEdit
            height: 25
            anchors {
                top: unitOwnerLabel.bottom
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

            model: W.map.allPlayers;
            textRole: "displayName"

            onActivated: {
                if (unitEdit.unit) {
                    var players = W.map.players;
                    if (index == 0) {
                        var owner = W.map.neutralPlayer;
                    } else {
                        var owner = players[index - 1];
                    }

                    unitEdit.unit.owner = owner;
                }
            }
        }
    }
}
