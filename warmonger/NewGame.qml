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
            id: campaignMapDelegate

            Rectangle {
                id: wrapper

                width: campaignMapList.width
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

                        text: model.modelData.displayName
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
                    id: campaignMapList

                    currentIndex: -1

                    model: W.campaignMaps

                    delegate: campaignMapDelegate
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
        }

        color: W.normalPalette.window
    }
}
