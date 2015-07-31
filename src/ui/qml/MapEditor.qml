import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

import "js/MapEditor.js" as MapEditor

Rectangle {
    Component.onCompleted: {
        MapEditor.init()
    }
    Rectangle {
        id: mapEditorStatusBar
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: mapEditorSideBar.left
        height: 40
        color: "green"
    }

    Map {
        id: mapEditorMap
        anchors {
            left: parent.left
            top: mapEditorStatusBar.bottom
            right: mapEditorSideBar.left
            bottom: parent.bottom
        }

        border {
            width: 1
            color: "black"
        }
    }

    Rectangle {
        id: mapEditorSideBar
        anchors {
            top: parent.top
            right: parent.right
            bottom: parent.bottom
        }

        width: 300
        color: "blue"

        Rectangle {
            id: mapEditorMiniMap
            anchors {
                left: parent.left
                top: parent.top
                right: parent.right
            }

            height: 300
            color: "yellow"
        }

        Rectangle {
            id: mapEditorControls
            anchors {
                top: mapEditorMiniMap.bottom
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }

            TabView {
                anchors.fill: parent

                Tab {
                    title: "Terrain Types"

                    Rectangle {
                        id: terrainTypeItems

                        GridLayout {
                            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                            columns: 7
                        }
                    }
                }
                Tab {
                    title: "Settlements"

                    Rectangle {
                    }
                }
                Tab {
                    title: "Units"

                    Rectangle {
                    }
                }
            }
        }
    }
}
