import QtQuick 2.2

import "js/Map.js" as Map

Rectangle {
    id: root

    property var game
    property var windowPos
    property var currentMapNode
    property var focusedMapNode

    currentMapNode: canvas.currentMapNode
    focusedMapNode: canvas.focusedMapNode

    signal windowChanged(var window)

    function onMiniMapWindowPosChanged(windowPos) {
        if (jobj !== undefined) {
            jobj.onWindowPosChanged(windowPos);
        }
    }

    onWindowPosChanged: {
        if (canvas.jobj) {
            canvas.jobj.onWindowPosChanged(windowPos);
        }
    }

    border {
        width: 1
        color: "black"
    }

    Canvas {
        id: canvas

        property var jobj
        property var currentMapNode
        property var focusedMapNode

        anchors.fill: parent

        Component.onCompleted: {
            Map.W = W;
            root.gameChanged.connect(onGameChanged);
            onGameChanged();
        }
        onCanvasWindowChanged: {
            root.windowChanged(canvasWindow);
        }
        onImageLoaded: {
            if (jobj) {
                jobj.onImageLoaded();
            }
        }
        onPaint: {
            if (jobj) {
                jobj.onPaint(region);
            }
        }

        function onGameChanged() {
            if (root.game) {
                jobj = new Map.GameMap(root.game, canvas, mouseArea);
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
