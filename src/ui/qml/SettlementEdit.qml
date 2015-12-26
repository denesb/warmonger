import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Rectangle {
    id: settlementEdit
    property var settlement

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
            if (settlementEdit.settlement) {
                var settlementTypeName = settlementEdit.settlement.settlementType.objectName;
                surface.bigMap[settlementTypeName];
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
                var label = "Settlement ";
                if (settlementEdit.settlement) {
                    label += settlementEdit.settlement.objectName;
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
                if (settlementEdit.settlement) {
                    settlementEdit.settlement.displayName;
                } else {
                    "";
                }
            }

            onValueEdited: {
                if (settlementEdit.settlement) {
                    settlementEdit.settlement.displayName = val;
                }
            }
        }

        ListEditGroup {
            id: settlementTypeEdit
            anchors {
                top: displayNameEdit.bottom
                left: parent.left
                right: parent.right
                topMargin: 10
            }

            label: "Settlement Type"
            model: W.map.world.settlementTypes
            currentIndex: {
                if (settlementEdit.settlement) {
                    settlementTypeEdit.find(settlementEdit.settlement.settlementType.displayName);
                } else {
                    0;
                }
            }

            onCurrentItemChanged: {
                if (settlementEdit.settlement) {
                    console.log(index);
                    settlementEdit.settlement.settlementType = W.map.world.settlementTypes[index];
                }
            }
        }

        ListEditGroup {
            id: ownerEdit
            anchors {
                top: settlementTypeEdit.bottom
                left: parent.left
                right: parent.right
                topMargin: 10
            }

            label: "Owner"
            model: W.map.allPlayers
            currentIndex: {
                if (settlementEdit.settlement) {
                    ownerEdit.find(settlementEdit.settlement.owner.displayName);
                } else {
                    0;
                }
            }

            onCurrentItemChanged: {
                if (settlementEdit.settlement) {
                    settlementEdit.settlement.owner = W.map.allPlayers[index];
                }
            }
        }
    }
}
