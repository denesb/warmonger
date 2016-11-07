import QtQuick 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
import QtQuick.Window 2.2
import Warmonger 1.0

Window {
    id: root

    minimumWidth: 400
    minimumHeight: 400

    title: "Warmonger Campaign Map Editor - Edit Factions"

    color: W.normalPalette.window

    property var editor

    Component {
        id: factionDelegate

        Rectangle {
            color: W.normalPalette.window

            width: factionList.width
            height: 40

            RowLayout {
                anchors.fill: parent

                spacing: 4

                TextField {
                    anchors {
                        verticalCenter: parent.verticalCenter
                    }

                    Layout.fillWidth: true

                    textColor: W.normalPalette.windowText

                    text: model.modelData.displayName

                    onEditingFinished: {
                        model.modelData.displayName = text
                    }
                }

                ComboBox {
                    anchors {
                        verticalCenter: parent.verticalCenter
                    }

                    model: W.world.civilizations
                    textRole: "displayName"

                    onActivated: {
                        model.modelData.civilization = W.world.civilizations[index];
                    }
                }

                Rectangle {
                    height: 30
                    width: 30

                    border {
                        width: 1
                        color: W.normalPalette.mid
                    }

                    anchors {
                        verticalCenter: parent.verticalCenter
                    }

                    Banner {
                        anchors {
                            fill: parent
                            margins: 1
                        }

                        banner: model.modelData.banner
                        primaryColor: model.modelData.primaryColor
                        secondaryColor: model.modelData.secondaryColor
                        worldSurface: W.worldSurface
                    }

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            bannerDialog.faction = model.modelData
                            bannerDialog.visible = true
                        }
                    }
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

    BannerDialog {
        id: bannerDialog

        visible: false

        faction: W.campaignMap.factions[0]
    }
}
