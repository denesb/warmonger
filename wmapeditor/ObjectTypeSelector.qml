/**
 * \copyright (C) 2015-2016 Botond Dénes
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
import Warmonger 1.0

Rectangle {
    id: root

    property var objectTypes
    property var objectType : null

    color: W.normalPalette.window

    Component {
        id: objectTypeComponent

        Rectangle {
            id: frame

            width: 84
            height: 84

            state: frame.GridView.isCurrentItem ? "current" : "normal"

            border {
                width: 1
                color: W.normalPalette.mid
            }

            color: W.normalPalette.window

            Image {
                anchors {
                    fill: parent
                    margins: 4
                }

                source: W.worldSurface.getObjectImageUrl(model.modelData)
                fillMode: Image.PreserveAspectFit
            }

            MouseArea {
                anchors.fill: parent

                acceptedButtons: Qt.LeftButton
                hoverEnabled: true

                onClicked: {
                    frame.GridView.view.currentItem.state = "normal"
                    frame.GridView.view.currentIndex = index
                }

                onEntered: {
                    frame.state = frame.GridView.isCurrentItem ? "currentFocused" : "normalFocused";
                }

                onExited: {
                    frame.state = frame.GridView.isCurrentItem ? "current" : "normal";
                }
            }

            states: [
                State {
                    name: "normal"
                    PropertyChanges {
                        target: frame;
                        color: W.normalPalette.window;
                        border.color: W.normalPalette.mid
                        border.width: 1
                    }
                },
                State {
                    name: "normalFocused"
                    PropertyChanges {
                        target: frame;
                        color: W.normalPalette.mid;
                        border.color: W.normalPalette.mid
                        border.width: 1
                    }
                },
                State {
                    name: "current"
                    PropertyChanges {
                        target: frame;
                        color: W.normalPalette.window;
                        border.color: W.normalPalette.dark;
                        border.width: 2
                    }
                },
                State {
                    name: "currentFocused"
                    PropertyChanges {
                        target: frame;
                        color: W.normalPalette.mid;
                        border.color: W.normalPalette.dark
                        border.width: 2
                    }
                }
            ]
        }
    }

    GridView {
        id: grid

        anchors {
            top: parent.top
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
            margins: 4
        }

        cellHeight: 88
        cellWidth: 88

        width: parent.width - parent.width % 88

        currentIndex: 0
        onCurrentIndexChanged: {
            root.objectType = root.objectTypes[currentIndex];
        }

        model: root.objectTypes
        delegate: objectTypeComponent
    }
}
