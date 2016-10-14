import QtQuick 2.2
import Warmonger 1.0
import "../widgets" as Widgets

Rectangle {
    id: root

    property var windowRect
    readonly property var windowPos: miniMap.windowPos

    state: "expanded"

    color: W.colorPalette.backgroundColor1

    border {
        color: W.colorPalette.foregroundColor1
        width: 1
    }

    CampaignMiniMap {
        id: miniMap

        campaignMap: W.campaignMap
        worldSurface: W.worldSurface
        windowRect: root.windowRect

        anchors.fill: parent
        anchors.margins: 1
    }

    Widgets.Button {
        id: button

        height: 20
        anchors {
            bottom: root.top
            left: root.left
            right: root.right
        }

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
            PropertyChanges { target: root; y: parent.y + parent.height - root.height }
            PropertyChanges { target: button; anchors.bottomMargin: -1; text: "⇣"}
        },
        State {
            name: "collapsed"
            PropertyChanges { target: root; y: parent.y + parent.height + 1}
            PropertyChanges { target: button; anchors.bottomMargin: 0; text: "⇡"}
        }
    ]

    transitions: [
        Transition {
            from: "expanded"
            to: "collapsed"
            NumberAnimation { easing.type: Easing.OutCubic; properties: "y"; duration: 200 }
        },
        Transition {
            from: "collapsed"
            to: "expanded"
            NumberAnimation { easing.type: Easing.OutCubic; properties: "y"; duration: 200 }
        }
    ]
}
