import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Rectangle {
    id: settlementEdit
    property var settlement

    onSettlementChanged: {
        var index = -1;
        if (settlementEdit.settlement) {
            index = settlementTypeEdit.find(settlementEdit.settlement.settlementType.displayName);
        }
        settlementTypeEdit.currentIndex = index;
    }

    Image {
        id: picture
        width: 64
        height: 64
        anchors {
            top: parent.top
            left: parent.left
            margins: 2
        }
    
        source: {
            if (settlementEdit.settlement) {
                var surface = W.map.world.surface;
                var settlementTypeName = settlementEdit.settlement.settlementType.objectName;
                surface.path + "/" + surface.gameMap[settlementTypeName];
            } else {
                ''
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
                var label = "Map Node ";
                if (settlementEdit.settlement) {
                    label += settlementEdit.settlement.objectName;
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
                if (settlementEdit.settlement) {
                    settlementEdit.settlement.displayName
                } else {
                    ''
                }
            }

            onEditingFinished: {
                if (settlementEdit.settlement) {
                    settlementEdit.settlement.displayName = displayNameEdit.text;
                }
            }
        }

        Label {
            id: settlementTypeLabel
            anchors {
                top: displayNameEdit.bottom
                left: parent.left
                right: parent.right
                topMargin: 10
            }
            color: "black"
            text: "Settlement Type:"
        }

        ComboBox {
            id: settlementTypeEdit
            height: 25
            anchors {
                top: settlementTypeLabel.bottom
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

            model: W.map.world.settlementTypes
            textRole: "displayName"

            onActivated: {
                if (settlementEdit.settlement) {
                    var settlementTypes = W.map.world.settlementTypes;
                    var settlementType = settlementTypes[index];

                    settlementEdit.settlement.settlementType = settlementType;
                }
            }
        }
    }
}
