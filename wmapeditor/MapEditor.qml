/**
 * Copyright (C) 2015-2017 Botond Dénes
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

import QtQuick 2.2
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.2
import QtQuick.Window 2.2
import Warmonger 1.0

ApplicationWindow {
    id: window
    visible: true
    visibility: Window.Maximized

    Component.onCompleted: {
        W.create(W.worlds[0]);
    }

    title: "Warmonger Campaign Map Editor"

    Action {
        id: newAction
        text: "&New Campaign Map"
        iconSource: "icons/document-new.svg"
        tooltip: "New Campaign Map"
        onTriggered: {
            setStatus("Creating new campaign map...")
            W.create(W.worlds[0]);
            setTemporaryStatus("Created new map with name: " + W.campaignMap.displayName)
        }
    }

    Action {
        id: openAction
        text: "O"
        tooltip: "Open Campaign Map"
        onTriggered: {
            campaignMapsWindow.accepted.connect(onAccepted);
            campaignMapsWindow.rejected.connect(onRejected);

            campaignMapsWindow.visible = true;
        }

        function onAccepted() {
            setStatus("Opening campaign map...");

            console.log(campaignMapsWindow.campaignMap);
            W.campaignMap = campaignMapsWindow.campaignMap;
            console.log("accept");

            campaignMapsWindow.accepted.disconnect(onAccepted);
            campaignMapsWindow.rejected.disconnect(onRejected);
        }

        function onRejected() {
            campaignMapsWindow.accepted.disconnect(onAccepted);
            campaignMapsWindow.rejected.disconnect(onRejected);
        }
    }

    Action {
        id: saveAction
        text: "S"
        tooltip: "Save Campaign Map"
        onTriggered: {
            setStatus("Saving...")
            if (W.save()) {
                setTemporaryStatus("Map saved to " + W.getLastPath());
            } else {
                setTemporaryStatus("Failed to save map to " + W.getLastPath());
                errorDialog.show(W.getLastErrorCategory(), W.getLastErrorMessage());
            }
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

    /* QtQuick menues don't work under Ubuntu/Unity
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
    */

    toolBar: ToolBar {
        RowLayout {
            anchors.fill: parent

            ToolButton {
                action: newAction
            }

            ToolButton {
                action: openAction
            }

            ToolButton {
                action: saveAction
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
                }
            }
        }
    }

    statusBar: StatusBar {
        Label {
            id: statusLabel

            anchors.fill: parent

            text: "Ready"
        }
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

    CampaignMapsWindow {
        id: campaignMapsWindow

        campaignMaps: W.campaignMaps

        visible: false
    }

    FactionsWindow {
        id: factionsWindow

        visible: false

        editor: mapEditor
    }

    MessageDialog {
        id: errorDialog
        visible: false
        standardButtons: StandardButton.Ok

        function show(errorCategory, errorMessage) {
            if (errorCategory == Context.IOError)
                this.title = "I/O Error";
            else if (errorCategory == Context.ContentError)
                this.title = "Content Error";
            else if (errorCategory == Context.InternalError)
                this.title = "Internal Error";
            else
                this.title = "Unknown Error";

            this.text = errorMessage;

            this.open();
        }
    }

    Timer {
        id: statusTimer

        interval: 5000
        repeat: false

        onTriggered: {
            statusLabel.text = "Ready"
        }
    }

    function setStatus(status) {
        statusLabel.text = status;

        if (statusTimer.running)
            statusTimer.stop();
    }

    function setTemporaryStatus(status) {
        statusLabel.text = status;

        if (statusTimer.running)
            statusTimer.restart();
        else
            statusTimer.start();
    }
}
