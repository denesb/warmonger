import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

Rectangle {
    id: root

    width: 512

    property var maps
    signal mapSelected(var map)

    border {
        width: 2
        color: "black"
    }

    Component {
        id: listItemComponent

        Rectangle {
            id: listItem
            width: 512
            height: 36

            border {
                width: 2
                color: "black"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    list.currentIndex = index;
                    mapSelected(model.modelData);
                }
            }

            Row {
                anchors.fill: parent
                anchors.leftMargin: 2
                anchors.rightMargin: 2

                Rectangle {
                    id: players

                    width: 32
                    height: 36
                    border {
                        width: 1
                        color: "black"
                    }

                    Text {
                        anchors.centerIn: parent
                        color: listItem.ListView.isCurrentItem ? "orange" : "black"
                        text: model.modelData.players.length
                    }
                }

                Rectangle {
                    id: size

                    width: 64
                    height: 36
                    border {
                        width: 1
                        color: "black"
                    }

                    Text {
                        anchors.centerIn: parent
                        color: listItem.ListView.isCurrentItem ? "orange" : "black"
                        text: model.modelData.mapNodes.length
                    }
                }

                Rectangle {
                    id: name

                    width: 256
                    height: 36
                    border {
                        width: 1
                        color: "black"
                    }

                    Text {
                        anchors.centerIn: parent
                        color: listItem.ListView.isCurrentItem ? "orange" : "black"
                        text: model.modelData.displayName
                    }
                }

                Rectangle {
                    id: world

                    width: 156
                    height: 36
                    border {
                        width: 1
                        color: "black"
                    }

                    Text {
                        anchors.centerIn: parent
                        color: listItem.ListView.isCurrentItem ? "orange" : "black"
                        text: model.modelData.world.displayName
                    }
                }
            }
        }
    }

    Rectangle {
        id: header
        z: 1

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        height: 36
        border {
            width: 2
            color: "black"
        }

        Row {
            anchors {
                fill: parent
                leftMargin: 2
                rightMargin: 2
            }

            Rectangle {
                id: players

                width: 32
                height: 36
                border {
                    width: 1
                    color: "black"
                }

                Text {
                    anchors.centerIn: parent
                    color: "black"
                    text: "#P"
                }
            }

            Rectangle {
                id: size

                width: 64
                height: 36
                border {
                    width: 1
                    color: "black"
                }

                Text {
                    anchors.centerIn: parent
                    color: "black"
                    text: "#N"
                }
            }

            Rectangle {
                id: name

                width: 256
                height: 36
                border {
                    width: 1
                    color: "black"
                }

                Text {
                    anchors.centerIn: parent
                    color: "black"
                    text: "Map Name"
                }
            }

            Rectangle {
                id: world

                width: 156
                height: 36
                border {
                    width: 1
                    color: "black"
                }

                Text {
                    anchors.centerIn: parent
                    color: "black"
                    text: "World"
                }
            }
        }
    }

    ListView {
        id: list

        anchors {
            top: header.bottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        currentIndex: -1
        model: root.maps
        delegate: listItemComponent
    }
}
