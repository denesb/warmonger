import QtQuick 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
import QtQuick.Window 2.2
import Warmonger 1.0

Window {
    id: root

    minimumWidth: 500
    minimumHeight: 500

    color: W.normalPalette.window

    property var editor

    Component {
        id: factionDelegate

        Rectangle {
            color: W.normalPalette.window

            width: factionList.width
            height: 30

            Row {
                anchors.fill: parent

                spacing: 4

                Text {
                    anchors {
                        verticalCenter: parent.verticalCenter
                    }

                    color: W.normalPalette.windowText

                    text: model.modelData.displayName
                }

                Banner {
                    height: 30
                    width: 30

                    banner: model.modelData.banner
                    primaryColor: model.modelData.primaryColor
                    secondaryColor: model.modelData.secondaryColor
                    worldSurface: W.worldSurface
                }

                Rectangle {
                    height: 30
                    width: 30

                    color: model.modelData.primaryColor
                }

                Rectangle {
                    height: 30
                    width: 30

                    color: model.modelData.secondaryColor
                }
            }
        }
    }

    Rectangle {
        id: factionListWrapper

        anchors {
            top: parent.top
            bottom: controls.top
            left: parent.left
            right: parent.right
            margins: 5
        }

        color: W.normalPalette.window

        ScrollView {
            anchors.fill: parent

            ListView {
                id: factionList

                spacing: 4

                model: W.campaignMap.factions

                delegate: factionDelegate
            }
        }
    }

    RowLayout {
        id: controls

        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
            margins: 5
        }

        Label {
            text: "Number of factions:"
        }

        SpinBox {
            value: W.campaignMap.factions.length
            onEditingFinished: {
                editor.setNumberOfFactions(value);
            }
        }

        Item {
            Layout.fillWidth: true
        }

        Button {
            text: "Close"
            onClicked: {
                factionsWindow.visible = false;
            }
        }
    }
}
