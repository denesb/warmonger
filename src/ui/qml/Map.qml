import QtQuick 2.2

import "js/Map.js" as Map

Rectangle {
    id: map

    signal clicked(var mouse)
    signal positionChanged(var mouse)

    property var mapObject: Map.newMap(warmonger.map, mapCanvas, warmonger)

    Component.onCompleted: {
        map.mapObject = Map.newMap(warmonger.map, mapCanvas, warmonger)
    }
    Canvas {
        id: mapCanvas
        anchors.fill: parent
        onPaint: map.mapObject.paint(mapCanvas, region)
        onImageLoaded: map.mapObject.onResourceLoaded()

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            hoverEnabled: true

            onClicked: map.clicked(mouse)
            onPositionChanged: map.positionChanged(mouse)
        }
    }
}
