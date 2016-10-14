import QtQuick 2.2

Rectangle {
    id: root

    property string text

    height: 28

    color: W.colorPalette.backgroundColor0

    Rectangle {
        id: showText

        anchors.fill: parent

        color: W.colorPalette.backgroundColor0

        state: "active"

        Text {
            id: text

            anchors {
                verticalCenter: parent.verticalCenter
            }

            width: root.width - 24

            elide: Text.ElideRight

            text: root.text
        }

        Rectangle {
            anchors {
                verticalCenter: parent.verticalCenter
                right: parent.right
            }

            width: 24
            height: 24

            color: W.colorPalette.backgroundColor1

            Image {
                id: image

                anchors.fill: parent

                source: "qrc:///icons/edit.svg"
            }

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    showText.state = "inactive";
                    editText.state = "active";
                }
            }
        }

        states: [
            State {
                name: "active"
                PropertyChanges { target: showText; visible: true; enabled: true; }
            },
            State {
                name: "inactive"
                PropertyChanges { target: showText; visible: false; enabled: false; }
            }
        ]
    }

    Rectangle {
        id: editText

        anchors.fill: parent

        color: W.colorPalette.backgroundColor0

        state: "inactive"

        TextEdit {
            id: textEdit

            anchors {
                verticalCenter: parent.verticalCenter
            }

            width: root.width - 24

            text: root.text

            textFormat: TextEdit.PlainText
        }

        Rectangle {
            anchors {
                verticalCenter: parent.verticalCenter
                right: parent.right
            }

            width: 24
            height: 24

            color: W.colorPalette.backgroundColor1

            Image {
                anchors.fill: parent

                source: "qrc:///icons/ok.svg"
            }
        }

        states: [
            State {
                name: "active"
                PropertyChanges { target: editText; visible: true; enabled: true; }
            },
            State {
                name: "inactive"
                PropertyChanges { target: editText; visible: false; enabled: false; }
            }
        ]
    }
}
