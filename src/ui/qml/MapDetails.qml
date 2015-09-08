import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

import "js/Map.js" as Map

Rectangle {
    id: mapDetails

    property var map

    onMapChanged: {
        if (mapDetails.map)
            mapPreview.jobj = new Map.MapPreview(mapDetails.map, mapPreview);
        else
            mapPreview.jobj = undefined;
    }

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
        id: mapPreviewWrapper
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        height: 400

        border {
            width: 1
            color: "gold"
        }

        Canvas {
            id: mapPreview
            anchors {
                verticalCenter: parent.verticalCenter
                horizontalCenter: parent.horizontalCenter
            }
            width: 384
            height: 384

            property var jobj

            onPaint: {
                if (mapPreview.jobj)
                    mapPreview.jobj.onPaint(region);
            }
        }
    }

    Rectangle {
        anchors {
            top: mapPreviewWrapper.bottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        ColumnLayout {
            anchors.fill: parent

            Text {
                Layout.fillWidth: true
                Layout.preferredHeight: 15

                text: {
                    if (mapDetails.map)
                        mapDetails.map.displayName;
                    else
                        "";
                }
            }

            Text {
                Layout.fillWidth: true
                Layout.preferredHeight: 15

                text: {
                    if (mapDetails.map)
                        "World: " + mapDetails.map.world.displayName;
                    else
                        "";
                }
            }

            Text {
                Layout.fillWidth: true
                Layout.preferredHeight: 15

                text: {
                    if (mapDetails.map)
                        mapDetails.map.mapNodes.length + " nodes";
                    else
                        "";
                }
            }

            Text {
                Layout.fillWidth: true
                Layout.preferredHeight: 15

                text: {
                    if (mapDetails.map)
                        mapDetails.map.players.length + " players";
                    else
                        "";
                }
            }

            Text {
                Layout.fillWidth: true
                Layout.fillHeight: true

                text: {
                    if (mapDetails.map)
                        mapDetails.map.description;
                    else
                        "";
                }
            }
        }
    }
}
