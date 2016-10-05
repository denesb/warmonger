import QtQuick 2.2
import Warmonger 1.0
import "../components" as Components

Rectangle {
    id: root

    property var miniMapWindowRect
    readonly property var miniMapWindowPos: miniMap.windowPos
    readonly property var objectType: objectTypeSelector.objectType
    property var availableObjectTypes
    property var editingMode : tabList.model.get(0).mode

    color: W.colorPalette.backgroundColor0

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
        id: tabs

        color: W.colorPalette.backgroundColor1

        anchors {
            top: parent.top
            bottom: miniMapWrapper.top
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
                root.editingMode = model.get(currentIndex).mode
            }

            ListModel {
                id: editingModes

                ListElement {
                    name: "T"
                    mode: CampaignMapEditor.TerrainType
                }

                ListElement {
                    name: "S"
                    mode: CampaignMapEditor.SettlementType
                }

                ListElement {
                    name: "A"
                    mode: CampaignMapEditor.ArmyType
                }

                ListElement {
                    name: "E"
                    mode: CampaignMapEditor.Edit
                }

                ListElement {
                    name: "R"
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
            bottom: miniMapWrapper.top
            left: parent.left
            right: tabs.left
        }

        Components.ObjectTypeSelector {
            id: objectTypeSelector

            anchors.fill: parent

            objectTypes: root.availableObjectTypes
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
