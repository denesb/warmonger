import QtQuick 2.2
import Warmonger 1.0

import "../widgets" as Widgets

Rectangle {
    id: root

    property var objectTypes
    property var objectType : null

    color: W.colorPalette.backgroundColor0

    Component {
        id: objectTypeComponent

        Rectangle {
            id: frame

            width: 84
            height: 84

            state: frame.GridView.isCurrentItem ? "current" : "normal"

            border {
                width: 1
                color: W.colorPalette.foregroundColor1
            }

            color: W.colorPalette.backgroundColor1

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
                        color: W.colorPalette.backgroundColor1;
                        border.color: W.colorPalette.foregroundColor1
                        border.width: 1
                    }
                },
                State {
                    name: "normalFocused"
                    PropertyChanges {
                        target: frame;
                        color: W.colorPalette.foregroundColor1;
                        border.color: W.colorPalette.foregroundColor1
                        border.width: 1
                    }
                },
                State {
                    name: "current"
                    PropertyChanges {
                        target: frame;
                        color: W.colorPalette.backgroundColor1;
                        border.color: W.colorPalette.focusColor0;
                        border.width: 2
                    }
                },
                State {
                    name: "currentFocused"
                    PropertyChanges {
                        target: frame;
                        color: W.colorPalette.foregroundColor1;
                        border.color: W.colorPalette.focusColor0
                        border.width: 2
                    }
                }
            ]
        }
    }

    Component {
        id: editingTypeComponent

        Rectangle {
            id: frame

            width: 30
            height: 30

            state: frame.ListView.isCurrentItem ? "current" : "normal"

            border {
                width: 1
                color: W.colorPalette.foregroundColor1
            }

            color: W.colorPalette.backgroundColor1

            Text {
                anchors.centerIn: parent

                text: name
            }

            MouseArea {
                anchors.fill: parent

                acceptedButtons: Qt.LeftButton
                hoverEnabled: true

                onClicked: {
                    frame.ListView.view.currentItem.state = "normal"
                    frame.ListView.view.currentIndex = index
                }

                onEntered: {
                    frame.state = frame.ListView.isCurrentItem ? "currentFocused" : "normalFocused";
                }

                onExited: {
                    frame.state = frame.ListView.isCurrentItem ? "current" : "normal";
                }
            }

            states: [
                State {
                    name: "normal"
                    PropertyChanges {
                        target: frame;
                        color: W.colorPalette.backgroundColor1;
                        border.color: W.colorPalette.foregroundColor1
                        border.width: 1
                    }
                },
                State {
                    name: "normalFocused"
                    PropertyChanges {
                        target: frame;
                        color: W.colorPalette.foregroundColor1;
                        border.color: W.colorPalette.foregroundColor1
                        border.width: 1
                    }
                },
                State {
                    name: "current"
                    PropertyChanges {
                        target: frame;
                        color: W.colorPalette.backgroundColor1;
                        border.color: W.colorPalette.focusColor0;
                        border.width: 2
                    }
                },
                State {
                    name: "currentFocused"
                    PropertyChanges {
                        target: frame;
                        color: W.colorPalette.foregroundColor1;
                        border.color: W.colorPalette.focusColor0
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
