import QtQuick 2.2

import "js/Map.js" as Map

Rectangle {
    Canvas {
        id: mapCanvas
        anchors.fill: parent
        onPaint: Map.onPaint()
    }
}
