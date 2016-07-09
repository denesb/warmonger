import QtQuick 2.2
import QtQuick.Controls 1.1
import Warmonger 1.0

Rectangle {
    id: root
    anchors.fill: parent

    Component.onCompleted: {
        W.newMap();
    }

    MiniMap {
        id: miniMap

        map: W.map
        windowPos: map.windowPos
        windowSize: map.windowSize

        anchors {
            fill: parent
            margins: 1
        }
    }
}
