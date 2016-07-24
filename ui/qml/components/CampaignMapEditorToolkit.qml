import QtQuick 2.2
import "../widgets" as Widgets

Rectangle {
    id: root

    state: "expanded"

    color: W.colorPalette.backgroundColor1

    border {
        color: W.colorPalette.foregroundColor1
        width: 1
    }

    Widgets.Button {
        id: button

        width: 30
        height: 30
        anchors {
            right: root.left
            margins: -1
        }

        text: ">"

        onClicked: {
            if (root.state == "expanded") {
                root.state = "collapsed"
            } else {
                root.state = "expanded"
            }
        }
    }

    states: [
        State {
            name: "expanded"
            PropertyChanges { target: root; x: parent.x + parent.width - width }
            PropertyChanges { target: button; anchors.margins: -1; text: ">"}
        },
        State {
            name: "collapsed"
            PropertyChanges { target: root; x: parent.x + parent.width + 1}
            PropertyChanges { target: button; anchors.margins: 0; text: "<"}
        }
    ]

    transitions: [
        Transition {
            from: "expanded"
            to: "collapsed"
            NumberAnimation { easing.type: Easing.OutCubic; properties: "x"; duration: 200 }
        },
        Transition {
            from: "collapsed"
            to: "expanded"
            NumberAnimation { easing.type: Easing.OutCubic; properties: "x"; duration: 200 }
        }
    ]
}
