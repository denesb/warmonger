import QtQuick 2.2
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import Warmonger 1.0

Rectangle {
    id: root

    property var miniMapWindowRect
    readonly property var miniMapWindowPos: miniMap.windowPos
    readonly property var objectType: objectTypeSelector.objectType
    property var availableObjectTypes
    property var editingMode

    color: W.normalPalette.window

    ObjectTypeSelector {
        id: objectTypeSelector

        anchors {
            top: parent.top
            bottom: miniMapWrapper.top
            left: parent.left
            right: parent.right
        }

        objectTypes: root.availableObjectTypes
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
            color: W.normalPalette.mid
        }

        color: W.normalPalette.window

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
