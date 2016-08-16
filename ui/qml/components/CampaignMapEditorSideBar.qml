import QtQuick 2.2
import Warmonger 1.0
import "../components" as Components

Rectangle {
    id: root

    property var miniMapWindowRect
    readonly property var miniMapWindowPos: miniMap.windowPos
    readonly property var objectType: objectTypeSelector.objectType

    color: W.colorPalette.backgroundColor0

    Rectangle {
        id: toolkit

        color: W.colorPalette.backgroundColor0

        anchors {
            top: parent.top
            bottom: miniMapWrapper.top
            left: parent.left
            right: parent.right
        }

        Components.ObjectTypeSelector {
            id: objectTypeSelector

            anchors.fill: parent

            objectTypes: W.world.terrainTypes
        }
    }

    Rectangle {
        id: miniMapWrapper

        height: 200
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        border {
            width: 1
            color: W.colorPalette.foregroundColor1
        }

        color: W.colorPalette.backgroundColor1

        CampaignMiniMap {
            id: miniMap

            anchors {
                fill: parent
                margins: 1
            }

            campaignMap: W.campaignMap
            worldSurface: W.worldSurface
            windowRect: root.miniMapWindowRect
        }
    }
}
