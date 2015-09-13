import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

Rectangle {
    id: root

    property var stack

    Rectangle {
        id: mapListControls
        anchors {
            top: parent.top
            left: parent.left
            margins: 2
        }
        width: 512
        height: 36

        Row {
            anchors {
                left: parent.left
                right: parent.right
                verticalCenter: parent.verticalCenter
            }

            Button {
                text: "Back"
                onClicked: stack.pop()
            }
        }
    }

    MapList {
        id: mapList
        anchors {
            top: mapListControls.bottom
            bottom: parent.bottom
            left: parent.left
            margins: 2
        }

        maps: W.maps

        onMapSelected: W.newGame(map)
    }

    Rectangle {
        anchors {
            top: parent.top
            bottom: parent.bottom
            left: mapList.right
            right: parent.right
        }

        MapDetails {
            id: mapDetails

            map: W.game

            z: 1

            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }
        }

        GameSetup {
            id: gameSetup

            game: W.game

            anchors {
                top: mapDetails.bottom
                bottom: controls.top
                left: parent.left
                right: parent.right
            }
        }

        Row {
            id: controls
            anchors {
                bottom: parent.bottom
                left: parent.left
                right: parent.right
                leftMargin: 5
                rightMargin: 5
            }
            height: 30

            layoutDirection: Qt.RightToLeft

            Button {
                text: "Start Game"
                onClicked: {
                    root.stack.push({
                        item: Qt.createComponent("Game.qml"),
                        properties: {
                            game: gameSetup.game,
                            stack: root.stack
                        }
                    });
                }
            }
        }
    }
}
