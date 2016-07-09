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
        anchors {
            fill: parent
            margins: 1
        }
        border {
            width: 1
            color: "black"
        }
        color: "green"

        MiniMap {
            id: miniMap

            campaignMap: W.campaignMap
            worldSurface: W.worldSurface

            anchors {
                fill: parent
                margins: 1
            }
        }
    }
}
