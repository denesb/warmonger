import QtQuick 2.2
import QtQuick.Layouts 1.1
import Warmonger 1.0

Rectangle {
    id: root

    property var map

    height: 258
    anchors {
        margins: 2
    }
    border {
        width: 1
        color: "black"
    }

    Rectangle {
        id: mapPreviewWrapper
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

        MapPreview {
            id: mapPreview
            anchors {
                fill: parent
                margins: 2
            }

            map: root.map
        }
    }

    Rectangle {
        anchors {
            top: parent.top
            bottom: parent.bottom
            left: parent.left
            right: mapPreviewWrapper.left
            margins: 2
        }

        ColumnLayout {
            anchors.fill: parent

            Text {
                Layout.fillWidth: true
                Layout.preferredHeight: 15

                text: {
                    if (root.map)
                        "<b>" + root.map.displayName + "</b>";
                    else
                        "";
                }
            }

            Text {
                Layout.fillWidth: true
                Layout.preferredHeight: 15

                text: {
                    if (root.map)
                        "<i>" + root.map.world.displayName + "</i>";
                    else
                        "";
                }
            }

            Text {
                Layout.fillWidth: true
                Layout.preferredHeight: 15

                text: {
                    if (root.map)
                        root.map.mapNodes.length + " nodes";
                    else
                        "";
                }
            }

            Text {
                Layout.fillWidth: true
                Layout.preferredHeight: 15

                text: {
                    if (root.map)
                        root.map.players.length + " players";
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
                        if (root.map)
                            root.map.description;
                        else
                            "";
                    }
                }
            }
        }
    }
}
