import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

Rectangle {
    id: newGame

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

        onMapSelected: {
            mapDetails.map = map;
            gameSetup.game = W.newGame(mapDetails.map);
        }
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

            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }
        }

        GameSetup {
            id: gameSetup

            anchors {
                top: mapDetails.bottom
                bottom: parent.bottom
                left: parent.left
                right: parent.right
            }
        }
    }
}
