import QtQuick 2.2

import "js/Map.js" as Map

Rectangle {
    id: root

    readonly property var windowPos: canvas.windowPos

    property var map
    property var window

    onWindowChanged: {
        if (canvas.jobj) {
            canvas.jobj.setWindow(window)
        }
    }

    border {
        width: 1
        color: "black"
    }

    Canvas {
        id: canvas

        property var jobj
        property var windowPos

        anchors.fill: parent

        onPaint: {
            if (jobj) {
                jobj.onPaint(region);
            }
        }
        onImageLoaded: {
            if (jobj) {
                jobj.onResourceLoaded();
            }
        }
        Component.onCompleted: {
            root.mapChanged.connect(onMapChanged);
            onMapChanged();
        }

        function onMapChanged() {
            if (root.map) {
                jobj = new Map.MiniMap(root.map, canvas, mouseArea);
            } else {
                jobj = undefined;
            }
        }

        MouseArea {
            id: mouseArea

            anchors.fill: parent
            hoverEnabled: true
            acceptedButtons: Qt.LeftButton

            onPressed: {
                if (canvas.jobj) {
                    canvas.jobj.onPressed(mouse);
                }
            }
            onReleased: {
                if (canvas.jobj) {
                    canvas.jobj.onReleased(mouse);
                }
            }
            onPositionChanged: {
                if (canvas.jobj) {
                    canvas.jobj.onPositionChanged(mouse);
                }
            }
        }
    }
}
