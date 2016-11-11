import QtQuick 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
import Warmonger 1.0

ApplicationWindow {
    id: window
    visible: true

    Component.onCompleted: {
        W.newCampaignMap(W.worlds[0]);
    }

    title: "Warmonger Campaign Map Editor"

    Action {
        id: newAction
        text: "&New Campaign Map"
        iconSource: "icons/document-new.svg"
        tooltip: "New Campaign Map"
        onTriggered: {
            W.newCampaignMap(W.worlds[0]);
        }
    }

    Action {
        id: terrainTypeEditingModeAction
        text: "Terrain Types"
        iconSource: "icons/terrain-types.svg"
        tooltip: "Terrain Type Editing Mode"
        onTriggered: {
            mapEditor.editingMode = CampaignMapEditor.TerrainType;
        }
    }

    Action {
        id: settlementTypeEditingModeAction
        text: "Settlement Types"
        iconSource: "icons/settlement-types.svg"
        tooltip: "Settlement Type Editing Mode"
        onTriggered: {
            mapEditor.editingMode = CampaignMapEditor.SettlementType;
        }
    }

    Action {
        id: armyTypeEditingModeAction
        text: "Army Types"
        iconSource: "icons/army-types.svg"
        tooltip: "Army Type Editing Mode"
        onTriggered: {
            mapEditor.editingMode = CampaignMapEditor.ArmyType;
        }
    }

    Action {
        id: editFactions
        text: "F"
        tooltip: "Edit Factions"
        onTriggered: {
            factionsWindow.visible = true
        }
    }

    Action {
        id: grantToCurrentFaction
        text: "G"
        tooltip: "Grant to current faction"
        onTriggered: {
            mapEditor.editingMode = CampaignMapEditor.GrantToCurrentFaction;
        }
    }

    menuBar: MenuBar {
        Menu {
            title: "Campaign Map"
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

    toolBar: ToolBar {
        RowLayout {
            anchors.fill: parent

            ToolButton {
                action: newAction
            }

            ToolButton {
                action: terrainTypeEditingModeAction
            }

            ToolButton {
                action: settlementTypeEditingModeAction
            }

            ToolButton {
                action: armyTypeEditingModeAction
            }

            ToolButton {
                action: editFactions
            }

            ToolButton {
                action: grantToCurrentFaction
            }

            Item {
                Layout.fillWidth: true
            }

            ComboBox {
                id: currentFactionSelector

                property var factions: W.campaignMap.factions

                Layout.preferredWidth: 150

                textRole: "displayName"

                onActivated: {
                    if (index == 0)
                        mapEditor.currentFaction = null;
                    else
                        mapEditor.currentFaction = W.campaignMap.factions[index];
                }

                onFactionsChanged: {
                    factions.unshift({"displayName": "None"});
                    model = factions;
                    console.log(factions);
                }
            }
        }
    }

    contentItem {
        minimumWidth: 600
        minimumHeight: 400
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

        color: W.normalPalette.window

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

            anchors.fill: parent
            anchors.margins: 1
        }
    }

    FactionsWindow {
        id: factionsWindow

        visible: false

        editor: mapEditor
    }
}
