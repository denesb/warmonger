import QtQuick 2.2
import QtQuick.Controls 1.1

Rectangle {
    property var mapNode

    Image {
        id: picture
        width: 64
        height: 64
        anchors {
            top: parent.top
            left: parent.left
            margins: 2
        }
    
        source: {
            var surface = W.map.world.surface;
            surface.path + "/" + surface.gameMap[mapNode.terrainType.objectName]
        }
    }

    Rectangle {
        anchors {
            top: parent.top
            bottom: parent.bottom
            left: picture.right
            right: parent.right
        }

        color: "red"
    }
}
