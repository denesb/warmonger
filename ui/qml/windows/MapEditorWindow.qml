import QtQuick 2.2
import Warmonger 1.0
import "../components" as Components

Rectangle {
    id: root

    Component.onCompleted: {
        W.newCampaignMap(W.worlds[0]);
    }

    anchors.fill: parent

    Components.CampaignMapEditorSideBar {
        id: sideBar

        miniMapWindowRect: mapEditor.windowRect

        width: 200
        anchors {
            top: parent.top
            bottom: parent.bottom
            right: parent.right
        }
    }

    Rectangle {
        id: mapWrapper

        color: W.colorPalette.backgroundColor1

        anchors {
            top: parent.top
            bottom: parent.bottom
            left: parent.left
            right: sideBar.left
        }

        CampaignMapEditor {
            id: mapEditor

            campaignMap: W.campaignMap
            worldSurface: W.worldSurface
            windowPos: sideBar.miniMapWindowPos
            objectType: sideBar.objectType

            anchors.fill: parent
            anchors.margins: 1
        }
    }
}
