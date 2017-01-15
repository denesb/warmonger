/**
 * Copyright (C) 2015-2016 Botond Dénes
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
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import QtQuick.Window 2.2
import Warmonger 1.0

ApplicationWindow {
    id: window
    visible: true
    visibility: Window.Maximized

    title: "Warmonger"

    CampaignMapPreview {
        anchors.fill: parent

        campaignMap: backgroundCampaignMap
        worldSurface: W.worldSurface

        Rectangle {
            anchors {
                top: parent.top
                right: parent.right
                topMargin: 20
                rightMargin: 20
            }
            width: 200
            height: 200

            ColumnLayout {
                anchors.fill: parent

                Component {
                    id: menuButtonStyle

                    ButtonStyle {
                        background: Rectangle {
                            implicitWidth: 200
                            implicitHeight: 20
                            radius: 4
                            border.width: 1
                            border.color: "#888"
                            gradient: Gradient {
                                GradientStop { position: 0 ; color: control.pressed ? "#ccc" : "#eee" }
                                GradientStop { position: 1 ; color: control.pressed ? "#aaa" : "#ccc" }
                            }
                        }
                    }
                }

                Button {
                    text: "New Game"

                    style: menuButtonStyle
                }

                Button {
                    text: "Continue Game"

                    style: menuButtonStyle
                }

                Button {
                    text: "Manual"

                    style: menuButtonStyle
                }

                Button {
                    text: "About"

                    style: menuButtonStyle
                }

                Button {
                    text: "Exit"

                    style: menuButtonStyle

                    onClicked: {
                        window.close();
                    }
                }
            }
        }
    }
}
