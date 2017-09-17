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
                    root.Stack.view.pop();
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
        id: listWrapper

        color: W.normalPalette.window

        anchors {
            top: topBar.bottom
            bottom: parent.bottom
            left: parent.left
        }
        width: 400

        Component {
            id: mapDelegate

            Rectangle {
                id: wrapper

                width: mapList.width
                height: 40

                color: {
                    if (wrapper.ListView.isCurrentItem)
                        W.normalPalette.highlight;
                    else if(index % 2)
                        W.normalPalette.base;
                    else
                        W.normalPalette.alternateBase;
                }

                Item {
                    anchors {
                        fill: parent
                        leftMargin: 4
                        rightMargin: 4
                    }

                    Text {
                        anchors.verticalCenter: parent.verticalCenter

                        color: wrapper.ListView.isCurrentItem ? W.normalPalette.highlightedText : W.normalPalette.windowText

                        text: model.modelData.name
                    }
                }

                MouseArea {
                    anchors.fill: parent

                    onClicked: {
                        wrapper.ListView.view.currentIndex = index;
                    }
                }
            }
        }

        Rectangle {
            anchors {
                fill: parent
                margins: 8
            }

            color: W.normalPalette.light

            ScrollView {
                anchors.fill: parent

                ListView {
                    id: mapList

                    currentIndex: -1

                    model: W.maps

                    delegate: mapDelegate
                }
            }
        }
    }

    Rectangle {
        id: detailsWrapper

        anchors {
            top: topBar.bottom
            bottom: parent.bottom
            left: listWrapper.right
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
                        model: mapList.currentIndex == -1 ? null : W.maps[mapList.currentIndex].factions

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

                                    textColor: W.normalPalette.windowText

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

            Item {
                Layout.preferredWidth: 400
                Layout.fillHeight: true

                Rectangle {
                    id: previewWrapper

                    anchors {
                        top: parent.top
                        left: parent.left
                        right: parent.right
                        margins: 8
                    }

                    height: width

                    border {
                        color: W.normalPalette.midlight
                        width: 1
                    }
                    radius: 4

                    color: W.normalPalette.light

                    MapView {
                        anchors.fill: parent

                        map: mapList.currentIndex == -1 ? null : W.maps[mapList.currentIndex]
                        worldSurface: W.worldSurface
                    }
                }
            }
        }
    }
}
