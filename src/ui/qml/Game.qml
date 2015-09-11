import QtQuick 2.2

import "js/Map.js" as Map

Rectangle {
    id: root

    property var game
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
                jobj = new Map.GameMap(root.game, mapCanvas, mapMouseArea);

                //miniMap.onWindowPosChanged.connect(jobj.onWindowPosChanged.bind(jobj));
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
