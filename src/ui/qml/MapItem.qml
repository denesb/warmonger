import QtQuick 2.2
import QtQuick.Controls 1.1

Rectangle {
    id: mapItem
    width: 256
    height: 40

    signal selected(string mapItemName)

    Rectangle {
        width: 248
        height: 36
        radius: 5
        border {
            width: 1
            color: mapItem.ListView.isCurrentItem ? "orange" : "black"
        }
        anchors {
            verticalCenter: parent.verticalCenter
            horizontalCenter: parent.horizontalCenter
        }

        Image {
            id: icon
            width: 32
            height: 32
            anchors {
                top: parent.top
                left: parent.left
                margins: 2
            }

            source: {
                ui.map.world.getResourcePath(model.modelData.objectName)
            }
        }
        Label {
            id: label
            anchors {
                right: parent.right
                left: icon.right
                leftMargin: 10
                verticalCenter: parent.verticalCenter
            }
            color: mapItem.ListView.isCurrentItem ? "orange" : "black"
            text: model.modelData.displayName
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent

            onClicked: {
                mapItem.ListView.view.currentIndex = index
                mapItem.selected(model.modelData.objectName)
            }
        }
    }
}
