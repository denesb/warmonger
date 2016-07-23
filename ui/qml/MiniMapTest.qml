import QtQuick 2.2
import QtQuick.Controls 1.1
import Warmonger 1.0

Rectangle {
    id: root
    anchors.fill: parent

    Component.onCompleted: {
        W.newCampaignMap(W.worlds[0]);
    }

    Rectangle {
        id: mapEditorWrap
        anchors {
            top: parent.top
            bottom: parent.bottom
            left: parent.left
            right:  miniMapWrap.left
        }
        border {
            width: 1
            color: "black"
        }
        color: "yellow"

        CampaignMapEditor {
            id: campaignMapEditor

            campaignMap: W.campaignMap
            worldSurface: W.worldSurface

            anchors {
                fill: parent
                margins: 1
            }
        }
    }

    Rectangle {
        id: miniMapWrap
        anchors {
            top: parent.top
            bottom: parent.bottom
            right:  parent.right
        }
        width: 400
        border {
            width: 1
            color: "black"
        }
        color: "green"

        MiniMap {
            id: miniMap

            campaignMap: W.campaignMap
            worldSurface: W.worldSurface
            windowRect: campaignMapEditor.windowRect

            anchors {
                fill: parent
                margins: 1
            }
        }
    }
}
