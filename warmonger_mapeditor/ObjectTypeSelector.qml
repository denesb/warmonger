import QtQuick 2.2
import Warmonger 1.0

Rectangle {
    id: root

    property var objectTypes
    property var objectType : null

    color: W.normalPalette.window

    Component {
        id: objectTypeComponent

        Rectangle {
            id: frame

            width: 84
            height: 84

            state: frame.GridView.isCurrentItem ? "current" : "normal"

            border {
                width: 1
                color: W.normalPalette.mid
            }

            color: W.normalPalette.window

            Image {
                anchors {
                    fill: parent
                    margins: 4
                }

                source: W.worldSurface.getImageUrl(model.modelData)
                fillMode: Image.PreserveAspectFit
            }

            MouseArea {
                anchors.fill: parent

                acceptedButtons: Qt.LeftButton
                hoverEnabled: true

                onClicked: {
                    frame.GridView.view.currentItem.state = "normal"
                    frame.GridView.view.currentIndex = index
                }

                onEntered: {
                    frame.state = frame.GridView.isCurrentItem ? "currentFocused" : "normalFocused";
                }

                onExited: {
                    frame.state = frame.GridView.isCurrentItem ? "current" : "normal";
                }
            }

            states: [
                State {
                    name: "normal"
                    PropertyChanges {
                        target: frame;
                        color: W.normalPalette.window;
                        border.color: W.normalPalette.mid
                        border.width: 1
                    }
                },
                State {
                    name: "normalFocused"
                    PropertyChanges {
                        target: frame;
                        color: W.normalPalette.mid;
                        border.color: W.normalPalette.mid
                        border.width: 1
                    }
                },
                State {
                    name: "current"
                    PropertyChanges {
                        target: frame;
                        color: W.normalPalette.window;
                        border.color: W.normalPalette.dark;
                        border.width: 2
                    }
                },
                State {
                    name: "currentFocused"
                    PropertyChanges {
                        target: frame;
                        color: W.normalPalette.mid;
                        border.color: W.normalPalette.dark
                        border.width: 2
                    }
                }
            ]
        }
    }

    GridView {
        id: grid

        anchors {
            top: parent.top
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
            margins: 4
        }

        cellHeight: 88
        cellWidth: 88

        width: parent.width - parent.width % 88

        currentIndex: 0
        onCurrentIndexChanged: {
            root.objectType = root.objectTypes[currentIndex];
        }

        model: root.objectTypes
        delegate: objectTypeComponent
    }
}
