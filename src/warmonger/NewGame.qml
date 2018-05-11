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

import QtQuick 2.2
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

                text: "New Game"
            }
        }
    }

    Rectangle {
        id: mapOptions

        color: W.normalPalette.window

        anchors {
            top: topBar.bottom
            bottom: parent.bottom
            left: parent.left
        }
        width: 400

        Column {
            anchors {
                fill: parent
            }

            Label {
                text: "Size: " + sizeSlider.value
            }
            Slider {
                id: sizeSlider
                from: 8
                to: 16
                stepSize: 1

                onValueChanged: {
                    W.randomMapGenerator.size = this.value;
                }
            }

            Label {
                text: "Players: " + playersSlider.value
            }
            Slider {
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
        id: gameplayOptions

        anchors {
            top: topBar.bottom
            bottom: parent.bottom
            left: mapOptions.right
            right: parent.right
            topMargin: 8
            bottomMargin: 8
        }

        color: W.normalPalette.window

        RowLayout {
            anchors.fill: parent

            ScrollView {
                id: playerList

                Layout.fillWidth: true
                Layout.fillHeight: true

                Column {
                    Repeater {
                        model: W.randomMapGenerator.players

                        Rectangle {
                            color: W.normalPalette.window

                            width: playerList.width
                            height: 40

                            RowLayout {
                                anchors.fill: parent

                                spacing: 4

                                TextField {
                                    anchors {
                                        verticalCenter: parent.verticalCenter
                                    }

                                    Layout.fillWidth: true

                                    text: model.modelData.name

                                    onEditingFinished: {
                                        model.modelData.name = text
                                    }
                                }

                                ComboBox {
                                    anchors {
                                        verticalCenter: parent.verticalCenter
                                    }

                                    Layout.preferredWidth: 150

                                    model: W.world.civilizations
                                    textRole: "name"

                                    onActivated: {
                                        model.modelData.civilization = W.world.civilizations[index];
                                    }
                                }

                                Rectangle {
                                    Layout.preferredHeight: 30
                                    Layout.preferredWidth: 30

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
                }
            }
        }
    }
}
