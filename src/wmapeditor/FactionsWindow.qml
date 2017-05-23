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
import QtQuick.Layouts 1.2
import QtQuick.Window 2.2
import Warmonger 1.0

Window {
    id: root

    minimumWidth: 400
    minimumHeight: 400

    title: "Edit Factions"

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

                    text: model.modelData.name

                    onEditingFinished: {
                        model.modelData.name = text
                    }
                }

                ComboBox {
                    anchors {
                        verticalCenter: parent.verticalCenter
                    }

                    model: W.world.civilizations
                    textRole: "name"

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

                model: W.map.factions

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
            value: W.map.factions.length
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

        faction: W.map.factions[0]
    }
}
