import QtQuick 2.2
import QtQuick.Controls 1.1

Rectangle {
    id: mapItemSelector

    anchors.fill: parent

    property var model
    signal mapItemSelected(string objectName)

    Component {
        id: mapItemDelegate

        Rectangle {
            id: mapItem
            width: 256
            height: 40

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
                        var surface = W.map.world.surface;
                        surface.path + "/" + surface.gameMap[model.modelData.objectName]
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
                        mapItemSelector.mapItemSelected(model.modelData.objectName)
                    }
                }
            }
        }
    }

    ListView {
        id: mapItemList

        anchors.fill: parent

        currentIndex: -1

        model: mapItemSelector.model
        delegate: mapItemDelegate
    }
}
