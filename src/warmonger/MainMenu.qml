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
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import Warmonger 1.0

Rectangle {
    id: root

    color: W.normalPalette.window

    MapView {
        anchors.fill: parent

        map: backgroundMap
        worldSurface: W.worldSurface

        Text {
            anchors {
                top: parent.top
                horizontalCenter: parent.horizontalCenter
                topMargin: 20
            }

            font {
                pointSize: 18
            }

            color: W.normalPalette.windowText

            text: "Warmonger - " + W.world.name
        }

        Text {
            anchors {
                left: parent.left
                bottom: parent.bottom
                leftMargin: 20
                bottomMargin: 20
            }

            font {
                pointSize: 10
            }

            color: W.normalPalette.windowText

            text: W.version
        }

        Rectangle {
            anchors {
                bottom: parent.bottom
                right: parent.right
                bottomMargin: 20
                rightMargin: 20
            }
            width: 200
            height: 200
            radius: 4

            border {
                color: W.normalPalette.midlight
                width: 1
            }

            color: W.normalPalette.light

            ColumnLayout {
                anchors {
                    fill: parent
                    margins: 20
                }

                Button {
                    text: "New Game"

                    Layout.preferredWidth: 160
                    Layout.preferredHeight: 20

                    onClicked: {
                        root.StackView.view.push(Qt.resolvedUrl("NewGame.qml"));
                    }
                }

                Button {
                    text: "Continue Game"

                    Layout.preferredWidth: 160
                    Layout.preferredHeight: 20

                    enabled: false
                }

                Button {
                    text: "Manual"

                    Layout.preferredWidth: 160
                    Layout.preferredHeight: 20

                    enabled: false
                }

                Button {
                    text: "About"

                    Layout.preferredWidth: 160
                    Layout.preferredHeight: 20

                    enabled: false
                }

                Button {
                    text: "Exit"

                    Layout.preferredWidth: 160
                    Layout.preferredHeight: 20

                    onClicked: {
                        window.close();
                    }
                }
            }
        }
    }
}
