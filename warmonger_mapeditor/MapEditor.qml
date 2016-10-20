import QtQuick 2.2
import QtQuick.Controls 1.4
import Warmonger 1.0

ApplicationWindow {
    id: window
    visible: true

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
                        color: W.colorPalette.foregroundColor0;
                    }
                },
                State {
                    name: "active"
                    PropertyChanges {
                        target: resizeHandle;
                        color: W.colorPalette.focusColor0;
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

        color: W.colorPalette.backgroundColor1

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
