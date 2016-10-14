import QtQuick 2.2
import QtQuick.Layouts 1.2
import Warmonger 1.0
import "../components" as Components
import "../widgets" as Widgets

Rectangle {
    id: root

    property var miniMapWindowRect
    readonly property var miniMapWindowPos: miniMap.windowPos
    readonly property var objectType: objectTypeSelector.objectType
    property var availableObjectTypes
    property var editingMode : tabList.model.get(0).mode

    color: W.colorPalette.backgroundColor0

    Component.onCompleted: {
        tabList.onCurrentIndexChanged()
    }

    Component {
        id: tabButtonComponent

        Rectangle {
            id: tabButton

            width: 32
            height: 32

            color: W.colorPalette.backgroundColor0

            state: tabButton.ListView.isCurrentItem ? "current" : "normal"

            Text {
                anchors.centerIn: parent

                text: name
            }

            MouseArea {
                anchors.fill: parent

                acceptedButtons: Qt.LeftButton
                hoverEnabled: true

                onClicked: {
                    tabButton.ListView.view.currentItem.state = "normal"
                    tabButton.ListView.view.currentIndex = index
                    tabButton.state = "current"
                }

                onEntered: {
                    tabButton.state = tabButton.ListView.isCurrentItem ? "currentFocused" : "normalFocused";
                }

                onExited: {
                    tabButton.state = tabButton.ListView.isCurrentItem ? "current" : "normal";
                }
            }

            states: [
                State {
                    name: "normal"
                    PropertyChanges {
                        target: tabButton;
                        color: W.colorPalette.backgroundColor1;
                    }
                },
                State {
                    name: "normalFocused"
                    PropertyChanges {
                        target: tabButton;
                        color: W.colorPalette.backgroundColor1;
                    }
                },
                State {
                    name: "current"
                    PropertyChanges {
                        target: tabButton;
                        color: W.colorPalette.backgroundColor0;
                    }
                },
                State {
                    name: "currentFocused"
                    PropertyChanges {
                        target: tabButton;
                        color: W.colorPalette.backgroundColor0;
                    }
                }
            ]
        }
    }

    Rectangle {
        id: titleWrapper

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        height: 32

        color: W.colorPalette.backgroundColor0;

        Text {
            id: title

            anchors.centerIn: parent

            text: "Title"
        }
    }

    Rectangle {
        id: contentWrapper

        anchors {
            top: titleWrapper.bottom
            bottom: miniMapWrapper.top
            left: parent.left
            right: parent.right
        }

        height: 32

        Rectangle {
            id: tabs

            color: W.colorPalette.backgroundColor1

            anchors {
                top: parent.top
                bottom: parent.bottom
                right: parent.right
            }

            width: 32

            ListView {
                id: tabList
                anchors.fill: parent
                spacing: 0
                orientation: ListView.Vertical

                currentIndex: 0
                onCurrentIndexChanged: {
                    var current = model.get(currentIndex);

                    root.editingMode = current.mode;

                    title.text = current.title;

                    if (current.type == "mapSettings") {
                        mapSettings.state = "active";
                        objectTypeSelector.state = "inactive";
                        factions.state = "inactive";
                    } else if (current.type == "faction") {
                        mapSettings.state = "inactive";
                        objectTypeSelector.state = "inactive";
                        factions.state = "active";
                    } else {
                        mapSettings.state = "inactive";
                        objectTypeSelector.state = "active";
                        factions.state = "inactive";
                    }
                }

                ListModel {
                    id: editingModes

                    ListElement {
                        name: "MS"
                        type: "mapSettings"
                        title: "Map Settings"
                        mode: CampaignMapEditor.None
                    }
                    ListElement {
                        name: "T"
                        type: "mapNode"
                        title: "Map Nodes"
                        mode: CampaignMapEditor.TerrainType
                    }
                    ListElement {
                        name: "S"
                        type: "settlement"
                        title: "Settlements"
                        mode: CampaignMapEditor.SettlementType
                    }
                    ListElement {
                        name: "A"
                        type: "army"
                        title: "Armies"
                        mode: CampaignMapEditor.ArmyType
                    }
                    ListElement {
                        name: "F"
                        type: "faction"
                        title: "Factions"
                        mode: CampaignMapEditor.None
                    }
                    ListElement {
                        name: "E"
                        type: "edit"
                        title: "Edit"
                        mode: CampaignMapEditor.Edit
                    }
                    ListElement {
                        name: "R"
                        type: "remove"
                        title: "Remove"
                        mode: CampaignMapEditor.Remove
                    }
                }

                model: editingModes
                delegate: tabButtonComponent
            }
        }

        Rectangle {
            id: content

            color: W.colorPalette.backgroundColor0

            anchors {
                top: parent.top
                bottom: parent.bottom
                left: parent.left
                right: tabs.left
            }

            Rectangle {
                id: mapSettings

                anchors {
                    fill: parent
                    margins: 4
                }

                color: W.colorPalette.backgroundColor0

                state: "inactive"

                GridLayout {
                    width: parent.width

                    columns: 2

                    columnSpacing: 10
                    rowSpacing: 4

                    Text {
                        text: "File"
                    }

                    Widgets.EditableText {
                        text: W.campaignMap.objectName;

                        Layout.fillWidth: true
                    }

                    Text {
                        text: "Name"
                    }

                    Widgets.EditableText {
                        text: W.campaignMap.displayName

                        Layout.fillWidth: true
                    }
                }

                states: [
                    State {
                        name: "active"
                        PropertyChanges { target: mapSettings; visible: true; enabled: true; }
                    },
                    State {
                        name: "inactive"
                        PropertyChanges { target: mapSettings; visible: false; enabled: false; }
                    }
                ]
            }

            Components.ObjectTypeSelector {
                id: objectTypeSelector

                anchors.fill: parent

                objectTypes: root.availableObjectTypes

                state: "inactive"

                states: [
                    State {
                        name: "active"
                        PropertyChanges { target: objectTypeSelector; visible: true; enabled: true; }
                    },
                    State {
                        name: "inactive"
                        PropertyChanges { target: objectTypeSelector; visible: false; enabled: false; }
                    }
                ]
            }

            Rectangle {
                id: factions

                anchors.fill: parent

                color: "red"

                state: "inactive"

                states: [
                    State {
                        name: "active"
                        PropertyChanges { target: factions; visible: true; enabled: true; }
                    },
                    State {
                        name: "inactive"
                        PropertyChanges { target: factions; visible: false; enabled: false; }
                    }
                ]
            }
        }
    }

    Rectangle {
        id: miniMapWrapper

        height: 200
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        border {
            width: 1
            color: W.colorPalette.foregroundColor1
        }

        color: W.colorPalette.backgroundColor1

        CampaignMiniMap {
            id: miniMap

            anchors {
                fill: parent
                margins: 1
            }

            campaignMap: W.campaignMap
            worldSurface: W.worldSurface
            windowRect: root.miniMapWindowRect
        }
    }
}
