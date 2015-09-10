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
        anchors {
            top: parent.top
            bottom: controls.top
            left: parent.left
            right: parent.right
        }

        model: {
            if (root.game)
                root.game.players;
            else
                [];
        }
        delegate: playerSetup
    }

    Row {
        id: controls
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }
        height: 36

        layoutDirection: Qt.RightToLeft

        Button {
            text: "Start Game"
            onClicked: {
                /*
                newGame.stack.push({
                    item: Qt.createComponent("Game.qml"),
                    properties: {
                        game: game,
                        stack: newGame.stack
                    }
                });
                */
            }
        }
    }
}
