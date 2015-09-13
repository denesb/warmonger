import QtQuick 2.2

import "js/Map.js" as Map

Rectangle {
    id: root

    property var window
    signal windowPosChanged(var windowPos)

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

        onWindowPosChanged: {
            root.windowPosChanged(windowPos);
        }

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
            W.mapChanged.connect(onMapChanged);
            onMapChanged();
        }

        function onMapChanged() {
            if (W.map) {
                jobj = new Map.MiniMap(W.map, canvas, mouseArea);
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
