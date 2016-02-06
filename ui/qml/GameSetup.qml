import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.0
import QtQuick.Controls.Styles 1.1

Rectangle {
    id: root

    property var game

    ColorDialog {
        id: colorDialog
        title: "Please choose a color"
    }

    Component {
        id: playerSetup

        Row {
            height: 30

            layoutDirection: Qt.LeftToRight
            spacing: 5

            Label {
                anchors.verticalCenter: parent.verticalCenter
                text: "#" + index
            }

            TextField {
                anchors.verticalCenter: parent.verticalCenter
                text: model.modelData.displayName
                onTextChanged: {
                    model.modelData.displayName = text;
                }
            }

            Rectangle {
                anchors.verticalCenter: parent.verticalCenter
                id: playerColor

                width: 22
                height: width
                radius: 4

                border {
                    width: 1
                    color: "black"
                }
                color: model.modelData.color

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        colorDialog.accepted.connect(playerColor.setColor);
                        colorDialog.rejected.connect(playerColor.disconnectSignals);
                        colorDialog.currentColor = model.modelData.color;
                        colorDialog.open()
                    }
                }

                function setColor() {
                    model.modelData.color = colorDialog.currentColor;
                    playerColor.disconnectSignals();
                }

                function disconnectSignals() {
                    colorDialog.accepted.disconnect(playerColor.setColor);
                    colorDialog.rejected.disconnect(playerColor.disconnectSignals);
                }
            }

            ComboBox {
                id: faction

                anchors.verticalCenter: parent.verticalCenter
                width: 128

                model: root.game.world.factions
                textRole: "displayName"

                currentIndex: find(model.modelData.faction.displayName)

                onActivated: {
                    model.modelData.faction = root.game.world.factions[index];
                }
            }
        }
    }

    ListView {
        anchors.fill: parent

        model: {
            if (root.game)
                root.game.players;
            else
                [];
        }
        delegate: playerSetup
    }
}
