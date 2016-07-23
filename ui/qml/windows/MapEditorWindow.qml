import QtQuick 2.2
import Warmonger 1.0

Rectangle {
    id: root

    Component.onCompleted: {
        W.newCampaignMap(W.worlds[0]);
    }

    anchors.fill: parent

    Rectangle {
        id: menuBar

        height: 40
        anchors {
            top: parent.top
            left: parent.left
            right: sideBar.left
        }
    }

    Rectangle {
        id: sideBar

        width: 300
        anchors {
            top: parent.top
            bottom: parent.bottom
            right: parent.right
        }

        Rectangle {
            id: miniMapWrapper

            height: 300
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }

            border {
                color: "orange"
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
        }
    }

    Rectangle {
        id: statusBar

        height: 20
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: sideBar.left
        }
    }

    Rectangle {
        id: mapWrapper

        anchors {
            top: menuBar.bottom
            bottom: statusBar.top
            left: parent.left
            right: sideBar.left
        }

        border {
            color: "black"
            width: 1
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
