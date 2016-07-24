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

        color: W.colorPalette.backgroundColor0

        height: 40
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
    }

    Rectangle {
        id: miniMapWrapper

        height: 300
        width: 300
        z: 1
        anchors {
            top: menuBar.bottom
            right: parent.right
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
            top: menuBar.bottom
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
