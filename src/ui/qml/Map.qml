import QtQuick 2.2

import "js/Map.js" as Map

Rectangle {
    Component.onCompleted: {
        warmonger.mapChanged.connect(Map.onMapChanged);
    }
    Canvas {
        id: mapCanvas
        anchors.fill: parent
        onPaint: Map.onPaint(region)
        onImageLoaded: Map.onImageLoaded()
    }
}
