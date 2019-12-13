/**
 * Copyright (C) 2015-2018 Botond Dénes
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

import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Warmonger 1.0

Rectangle {
    id: root

    color: W.normalPalette.window

    Rectangle {
        id: topBar

        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
        }
        height: 40

        color: W.normalPalette.dark

        Item {
            anchors {
                fill: parent
                margins: 4
            }

            Button {
                anchors {
                    left: parent.left
                    verticalCenter: parent.verticalCenter
                }

                text: "Back"

                onClicked: {
                    root.StackView.view.pop();
                }
            }

            Text {
                anchors.centerIn: parent

                color: W.normalPalette.windowText
                font.pointSize: 20
                font.weight: Font.Bold

                text: "New Game"
            }
        }
    }

    BannerDialog {
        id: bannerDialog

        visible: false

        faction: W.randomMapGenerator.players[0]
    }

    Rectangle {
        id: playerOptions

        color: W.normalPalette.window

        anchors {
            top: topBar.bottom
            bottom: parent.bottom
            left: parent.left
        }
        width: 800

        ColumnLayout {
            anchors.fill: parent

            Label {
                Layout.margins: 8
                text: "Player options"
                font.pointSize: 16
                font.weight: Font.Bold
                horizontalAlignment: Text.AlignHCenter
                color: W.normalPalette.text
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 60
                Layout.margins: 8
                color: W.normalPalette.window

                RowLayout {
                    anchors.fill: parent

                    Label {
                        Layout.alignment: Qt.AlignVCenter
                        text: "Players: " + playersSlider.value
                        color: W.normalPalette.text
                    }
                    Slider {
                        Layout.fillWidth: true
                        Layout.margins: 8
                        id: playersSlider
                        from: 2
                        to: 8
                        stepSize: 1

                        onValueChanged: {
                            W.randomMapGenerator.numOfPlayers = this.value;
                        }
                    }
                }
            }

            Rectangle {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.margins: 8
                color: W.normalPalette.light

                ScrollView {
                    id: playerList
                    anchors.fill: parent

                    Column {
                        spacing: 4

                        Repeater {
                            model: W.randomMapGenerator.players

                            Rectangle {
                                width: playerList.width
                                height: 40

                                RowLayout {
                                    anchors.fill: parent

                                    spacing: 4

                                    TextField {
                                        Layout.fillWidth: true

                                        text: model.modelData.name

                                        onEditingFinished: {
                                            model.modelData.name = text
                                        }
                                    }

                                    ComboBox {
                                        Layout.preferredWidth: 150

                                        model: W.world.civilizations
                                        textRole: "name"

                                        onActivated: {
                                            model.modelData.civilization = W.world.civilizations[index];
                                        }
                                    }

                                    Rectangle {
                                        Layout.preferredHeight: 40
                                        Layout.preferredWidth: 40

                                        border {
                                            width: 1
                                            color: W.normalPalette.mid
                                        }

                                        Banner {
                                            anchors {
                                                centerIn: parent
                                            }
                                            width: 30
                                            height: 30

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
                    }
                }
            }
        }
    }

    Rectangle {
        id: mapOptions

        anchors {
            top: topBar.bottom
            bottom: parent.bottom
            left: playerOptions.right
            right: parent.right
        }

        color: W.normalPalette.window

        ColumnLayout {
            anchors.fill: parent

            Label {
                Layout.margins: 8
                text: "Map options"
                font.pointSize: 16
                font.weight: Font.Bold
                horizontalAlignment: Text.AlignHCenter
                color: W.normalPalette.text
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 60
                Layout.margins: 8
                color: W.normalPalette.window

                RowLayout {
                    anchors.fill: parent

                    Label {
                        Layout.alignment: Qt.AlignVCenter
                        text: "Size: " + sizeSlider.value
                        color: W.normalPalette.text
                    }
                    Slider {
                        Layout.fillWidth: true
                        Layout.margins: 8
                        id: sizeSlider
                        from: 8
                        to: 16
                        stepSize: 1

                        onValueChanged: {
                            W.randomMapGenerator.size = this.value;
                        }
                    }
                }
            }

            Rectangle {
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.margins: 8
                color: W.normalPalette.light

                Item {
                    anchors {
                        left: parent.left
                        right: parent.right
                        bottom: parent.bottom
                    }
                    height: 60

                    RowLayout {
                        anchors {
                            fill: parent
                            margins: 8
                        }

                        Button {
                            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                            text: "Create"

                            onClicked: {
                                W.state = Context.Gameplay;
                                root.StackView.view.push(Qt.resolvedUrl("Gameplay.qml"));
                            }
                        }
                    }
                }
            }
        }
    }
}
