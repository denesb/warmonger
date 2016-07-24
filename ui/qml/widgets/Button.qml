import QtQuick 2.2

Rectangle {
    id: button

    state: "blurred"

    signal clicked()
    property string text

    border {
        width: 1
    }

    Text {
        id: text

        anchors.centerIn: parent

        text: button.text
    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton
        hoverEnabled: true

        onClicked: button.clicked()
        onEntered: {
            button.state = "focused"
        }
        onExited: {
            button.state = "blurred"
        }
    }

    states: [
        State {
            name: "blurred"
            PropertyChanges { target: button; color: W.colorPalette.backgroundColor1; border.color: W.colorPalette.foregroundColor1 }
            PropertyChanges { target: text; color: W.colorPalette.foregroundColor1 }
        },

        State {
            name: "focused"
            PropertyChanges { target: button; color: W.colorPalette.foregroundColor1; border.color: W.colorPalette.foregroundColor1 }
            PropertyChanges { target: text; color: W.colorPalette.backgroundColor1 }
        }
    ]
}
