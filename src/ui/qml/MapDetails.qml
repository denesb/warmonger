import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

import "js/Map.js" as Map

Rectangle {
    id: mapDetails

    property var map

    height: 258
    anchors {
        margins: 2
    }
    border {
        width: 1
        color: "black"
    }

    onMapChanged: {
        if (mapDetails.map)
            mapCanvas.jobj = new Map.MapPreview(mapDetails.map, mapCanvas);
        else
            mapCanvas.jobj = undefined;
    }

    Rectangle {
        id: mapPreview
        anchors {
            top: parent.top
            bottom: parent.bottom
            right: parent.right
            margins: 2
        }
        width: height

        border {
            width: 1
            color: "gold"
        }

        Canvas {
            id: mapCanvas
            anchors {
                fill: parent
                margins: 2
            }

            property var jobj

            onPaint: {
                if (mapCanvas.jobj)
                    mapCanvas.jobj.onPaint(region);
            }
        }
    }

    Rectangle {
        anchors {
            top: parent.top
            bottom: parent.bottom
            left: parent.left
            right: mapPreview.left
            margins: 2
        }

        ColumnLayout {
            anchors.fill: parent

            Text {
                Layout.fillWidth: true
                Layout.preferredHeight: 15

                text: {
                    if (mapDetails.map)
                        "<b>" + mapDetails.map.displayName + "</b>";
                    else
                        "";
                }
            }

            Text {
                Layout.fillWidth: true
                Layout.preferredHeight: 15

                text: {
                    if (mapDetails.map)
                        "<i>" + mapDetails.map.world.displayName + "</i>";
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

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                border {
                    width: 1
                    color: "black"
                }

                Text {
                    anchors.fill: parent
                    anchors.margins: 2
                    wrapMode: Text.WordWrap

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
}
