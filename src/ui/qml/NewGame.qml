import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

Rectangle {
    id: newGame

    property var stack

    Rectangle {
        id: mapListControls
        anchors {
            top: parent.top
            left: parent.left
            margins: 2
        }
        width: 512
        height: 36

        Row {
            anchors {
                left: parent.left
                right: parent.right
                verticalCenter: parent.verticalCenter
            }

            Button {
                text: "Back"
                onClicked: stack.pop()
            }
        }
    }

    Rectangle {
        id: mapList
        width: 512
        anchors {
            top: mapListControls.bottom
            bottom: parent.bottom
            left: parent.left
            margins: 2
        }
        border {
            width: 2
            color: "black"
        }

        ListView {
            anchors {
                fill: parent
                topMargin: 10
                bottomMargin: 10
            }

            currentIndex: -1
            model: W.maps
            delegate: MapListMapMeta {
                id: mapMeta

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        mapMeta.ListView.view.currentIndex = index;
                        mapDetails.map = model.modelData;
                    }
                }
            }
        }
    }

    Rectangle {
        anchors {
            top: parent.top
            bottom: parent.bottom
            left: mapList.right
            right: parent.right
        }

        MapListMapDetails {
            id: mapDetails

            anchors {
                top: parent.top
                bottom: controls.top
                left: parent.left
                right: parent.right
            }
        }

        Row {
            id: controls
            anchors {
                bottom: parent.bottom
                left: parent.left
                right: parent.right
            }
            height: 36

            layoutDirection: Qt.RightToLeft

            Button {
                text: "Setup Game"
            }
        }
    }
}
