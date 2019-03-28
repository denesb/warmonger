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
        anchors {
            top: parent.top
            bottom: parent.bottom
            left: parent.left
            right: panel.left
        }
        color: "red"
    }

    Rectangle {
        id: panel
        anchors {
            top: parent.top
            bottom: parent.bottom
            right: parent.right
        }
        width: 400

        color: "teal"

        ColumnLayout {
            anchors.fill: parent
            spacing: 0

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 400

                color: "green"
            }
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 40

                color: "grey"
                Label {
                    text: "Selected tile summary"
                }
            }
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 400

                color: "orange"
                Label {
                    text: "Current object details"
                }
            }
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true

                color: "yellow"
                Label {
                    text: "Kingdom overview"
                }
            }
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 40

                color: "brown"

                Row {
                    Label {
                        text: "Turn 1"
                    }
                    Button {
                        text: "End Turn"
                    }
                }
            }
        }
    }
}
