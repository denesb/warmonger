import QtQuick 2.2
import Warmonger 1.0
import "../widgets"

Rectangle {
    id: root

    Component.onCompleted: {
        W.newCampaignMap(W.worlds[0]);
    }

    anchors.fill: parent

    Rectangle {
        id: miniMapWrapper

        state: "expanded"

        width: 300
        height: 300
        z: 1
        anchors {
            top: parent.top
        }

        color: W.colorPalette.backgroundColor1

        border {
            color: W.colorPalette.foregroundColor1
            width: 1
        }

        CampaignMiniMap {
            id: miniMap

            campaignMap: W.campaignMap
            worldSurface: W.worldSurface
            windowRect: mapEditor.windowRect

            anchors.fill: parent
            anchors.margins: 1
        }

        WButton {
            id: miniMapTools

            width: 30
            height: 30
            anchors {
                right: miniMapWrapper.left
                margins: -1
            }

            text: ">"

            onClicked: {
                if (miniMapWrapper.state == "expanded") {
                    miniMapWrapper.state = "collapsed"
                } else {
                    miniMapWrapper.state = "expanded"
                }
            }
        }

        states: [
            State {
                name: "expanded"
                PropertyChanges { target: miniMapWrapper; x: parent.x + parent.width - width }
                PropertyChanges { target: miniMapTools; anchors.margins: -1; text: ">"}
            },
            State {
                name: "collapsed"
                PropertyChanges { target: miniMapWrapper; x: parent.x + parent.width + 1}
                PropertyChanges { target: miniMapTools; anchors.margins: 0; text: "<"}
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

    Rectangle {
        id: statusBar

        color: W.colorPalette.backgroundColor0

        height: 20
        anchors {
            bottom: parent.bottom
            left: parent.left
        }
    }

    Rectangle {
        id: mapWrapper

        color: W.colorPalette.backgroundColor1

        anchors {
            top: parent.top
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        CampaignMapEditor {
            id: mapEditor

            campaignMap: W.campaignMap
            worldSurface: W.worldSurface
            windowPos: miniMap.windowPos

            anchors.fill: parent
            anchors.margins: 1
        }
    }
}
