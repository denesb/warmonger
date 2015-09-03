import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

import "js/Map.js" as Map

Rectangle {
    id: newGame

    Rectangle {
        id: mapList
        width: 256
        anchors {
            top: parent.top
            bottom: parent.bottom
            left: parent.left
            margins: 2
        }
        border {
            width: 2
            color: "black"
        }

        Component {
            id: mapDelegate

            Rectangle {
                id: map

                width: 248
                height: 36
                radius: 5
                border {
                    width: 1
                    color: map.ListView.isCurrentItem ? "orange" : "black"
                }
                anchors {
                    verticalCenter: parent.verticalCenter
                    horizontalCenter: parent.horizontalCenter
                }

                Label {
                    id: mapLabel
                    anchors {
                        right: parent.right
                        left: parent.left
                        margins: 10
                        verticalCenter: parent.verticalCenter
                    }
                    color: map.ListView.isCurrentItem ? "orange" : "black"
                    text: model.modelData.displayName
                }

                MouseArea {
                    id: mouseArea
                    anchors.fill: parent

                    onClicked: {
                        map.ListView.view.currentIndex = index;
                        mapSetup.map = model.modelData;
                    }
                }
            }
        }

        ListView {
            anchors {
                fill: parent
                topMargin: 10
                bottomMargin: 10
            }

            currentIndex: -1
            model: W.mapList()
            delegate: mapDelegate
        }
    }

    Rectangle {
        id: mapSetup

        property var map

        anchors {
            top: parent.top
            bottom: parent.bottom
            left: mapList.right
            right: parent.right
            margins: 2
        }
        border {
            width: 2
            color: "black"
        }

        Rectangle {
            id: mapDetails
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }
            height: 384

            Rectangle {
                id: mapDescription

                anchors {
                    top: parent.top
                    bottom: parent.bottom
                    left: parent.left
                    right: mapPreview.left
                }

                ColumnLayout {
                    anchors.fill: parent

                    Text {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 15

                        text: {
                            if (mapSetup.map)
                                mapSetup.map.displayName;
                            else
                                "";
                        }
                    }

                    Text {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 15

                        text: {
                            if (mapSetup.map)
                                "World: " + mapSetup.map.world.displayName;
                            else
                                "";
                        }
                    }

                    Text {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 15

                        text: {
                            if (mapSetup.map)
                                mapSetup.map.mapNodes.length + " nodes";
                            else
                                "";
                        }
                    }

                    Text {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 15

                        text: {
                            if (mapSetup.map)
                                mapSetup.map.players.length + " players";
                            else
                                "";
                        }
                    }

                    Text {
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        text: {
                            if (mapSetup.map)
                                mapSetup.map.description;
                            else
                                "";
                        }
                    }
                }
            }

            Canvas {
                id: mapPreview
                anchors {
                    top: parent.top
                    bottom: parent.bottom
                    right: parent.right
                }
                width: 384

                /*
                Component.onCompleted: {
                    jobj = new Map.MapPreview(mapSetup.map, mapPreview, miniMapMouseArea);
                    jobj.windowPosChanged = miniMap.windowPosChanged;
                }

                onPaint: mapPreview.jobj.onPaint(region)
                onImageLoaded: mapPreview.jobj.onResourceLoaded()
                */
            }
        }

        Rectangle {
            id: mapOptions
            anchors {
                top: mapDetails.bottom
                bottom: mapControls.top
                left: parent.left
                right: parent.right
            }
            color: "green"
        }

        Rectangle {
            id: mapControls
            anchors {
                bottom: parent.bottom
                left: parent.left
                right: parent.right
            }
            height: 40
            color: "red"
        }
    }
}
