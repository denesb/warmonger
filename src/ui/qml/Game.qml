import QtQuick 2.2

import "js/Map.js" as Map

Rectangle {
    id: root

    property var stack

    Rectangle {
        id: statusBar

        anchors {
            top: parent.top
            left: parent.left
            right: sideBar.left
        }
        height: 40
    }

    Rectangle {
        id: sideBar

        anchors {
            top: parent.top
            bottom: parent.bottom
            right: parent.right
        }

        width: 256

        Rectangle {
            id: miniMap

            anchors {
                left: parent.left
                top: parent.top
                right: parent.right
            }
            height: 288
            border {
                width: 1
                color: "black"
            }

            Canvas {
                id: miniMapCanvas

                property var jobj
                property var windowPos

                anchors.fill: parent

                onPaint: jobj.onPaint(region)
                onImageLoaded: jobj.onResourceLoaded()
                Component.onCompleted: {
                    jobj = new Map.MiniMap(W.game, miniMapCanvas, miniMapMouseArea);
                    mapCanvas.onCanvasWindowChanged.connect(onMapCanvasWindowChanged);
                }

                function onMapCanvasWindowChanged() {
                    if (jobj != undefined)
                        jobj.setWindow(mapCanvas.canvasWindow)
                }

                MouseArea {
                    id: miniMapMouseArea

                    anchors.fill: parent
                    hoverEnabled: true
                    acceptedButtons: Qt.LeftButton

                    onPressed: miniMapCanvas.jobj.onPressed(mouse)
                    onReleased: miniMapCanvas.jobj.onReleased(mouse)
                    onPositionChanged: miniMapCanvas.jobj.onPositionChanged(mouse)
                }
            }
        }
    }

    Rectangle {
        id: map

        anchors {
            top: statusBar.bottom
            bottom: parent.bottom
            left: parent.left
            right: sideBar.left
        }

        border {
            width: 1
            color: "black"
        }

        Canvas {
            id: mapCanvas

            property var jobj

            anchors.fill: parent

            onPaint: jobj.onPaint(region)
            onImageLoaded: jobj.onResourceLoaded()
            Component.onCompleted: {
                jobj = new Map.GameMap(W.game, mapCanvas, mapMouseArea);

                miniMapCanvas.onWindowPosChanged.connect(
                    onMiniMapCanvasWindowPosChanged
                );
            }

            function onMiniMapCanvasWindowPosChanged() {
                if (jobj != undefined)
                    jobj.onWindowPosChanged(miniMapCanvas.windowPos);
            }

            MouseArea {
                id: mapMouseArea

                anchors.fill: parent
                hoverEnabled: true
                acceptedButtons: Qt.LeftButton

                onPressed: mapCanvas.jobj.onPressed(mouse)
                onReleased: mapCanvas.jobj.onReleased(mouse)
                onPositionChanged: mapCanvas.jobj.onPositionChanged(mouse)
            }
        }
    }
}
