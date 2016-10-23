import QtQuick 2.2
import QtQuick.Controls 1.4
import Warmonger 1.0

ApplicationWindow {
    id: window
    visible: true

    menuBar: MenuBar {
        Menu {
            title: "File"
            MenuItem { text: "New"; shortcut: "Ctrl+n" }
            MenuItem { text: "Open"; shortcut: "Ctrl+o" }
            MenuItem { text: "Save"; shortcut: "Ctrl+s" }
            MenuItem { text: "Save as"; shortcut: "Ctrl+Shitf+S" }
            MenuItem { text: "Close"; shortcut: "Ctrl+Q" }
        }
        Menu {
            title: "Edit"
            MenuItem { text: "Map Properties" }
        }
    }

    Component.onCompleted: {
        W.newCampaignMap(W.worlds[0]);
    }

    Rectangle {
        id: sideBarWrapper

        width: 250
        anchors {
            top: parent.top
            bottom: parent.bottom
            right: parent.right
        }

        Rectangle {
            id: resizeHandle

            anchors {
                top: parent.top
                bottom: parent.bottom
                left: parent.left
            }

            width: 3

            state: "normal"

            MouseArea {
                anchors.fill: parent

                acceptedButtons: Qt.LeftButton
                hoverEnabled: true

                cursorShape: Qt.SizeHorCursor

                onEntered: {
                    resizeHandle.state = "active";
                }

                onExited: {
                    if (!pressed)
                        resizeHandle.state = "normal";
                }

                onPressedChanged: {
                    if (!pressed)
                        resizeHandle.state = "normal"
                }

                onPositionChanged: {
                    if (mouse.buttons) {
                        var d = mouse.x - 2;
                        var newWidth = sideBarWrapper.width - d;

                        if (newWidth > 250)
                            sideBarWrapper.width = newWidth;
                    }
                }
            }

            states: [
                State {
                    name: "normal"
                    PropertyChanges {
                        target: resizeHandle;
                        color: W.normalPalette.window;
                    }
                },
                State {
                    name: "active"
                    PropertyChanges {
                        target: resizeHandle;
                        color: W.normalPalette.window;
                    }
                }
            ]
        }

        SideBar {
            id: sideBar

            miniMapWindowRect: mapEditor.windowRect
            availableObjectTypes: mapEditor.availableObjectTypes

            anchors {
                fill: parent
                leftMargin: 3
            }
        }
    }

    Rectangle {
        id: mapWrapper

        color: W.normalPalette.backgroundColor1

        anchors {
            top: parent.top
            bottom: parent.bottom
            left: parent.left
            right: sideBarWrapper.left
        }

        CampaignMapEditor {
            id: mapEditor

            campaignMap: W.campaignMap
            worldSurface: W.worldSurface
            windowPos: sideBar.miniMapWindowPos
            objectType: sideBar.objectType
            editingMode: sideBar.editingMode

            anchors.fill: parent
            anchors.margins: 1
        }
    }
}
