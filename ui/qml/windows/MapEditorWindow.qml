import QtQuick 2.2
import Warmonger 1.0
import "../components" as Components

Rectangle {
    id: root

    Component.onCompleted: {
        W.newCampaignMap(W.worlds[0]);
    }

    anchors.fill: parent

    Components.CampaignMiniMap {
        id: miniMap

        windowRect: mapEditor.windowRect

        width: 200
        height: 200
        z: 1

        border {
            color: W.colorPalette.foregroundColor1
            width: 1
        }

        anchors {
            right: parent.right
        }
    }

    /*
    Components.CampaignMapEditorToolkit {
        id: editingToolkit

        width: 300
        z: 1

        anchors {
            top: miniMap.bottom
            bottom: parent.bottom
        }
    }
    */

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
