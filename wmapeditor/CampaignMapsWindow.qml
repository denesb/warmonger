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

Window {
    id: root

    minimumWidth: 400
    minimumHeight: 400

    property var campaignMaps
    property var campaignMap
    signal accepted
    signal rejected

    title: "Open Campain Map"

    color: W.normalPalette.window

    property var editor

    Component {
        id: campaignMapDelegate

        Rectangle {
            id: wrapper

            color: ListView.isCurrentItem ? W.normalPalette.highlight : W.normalPalette.window

            width: campaignMapList.width
            height: 40

            Item {
                anchors {
                    fill: parent
                    leftMargin: 4
                    rightMargin: 4
                }

                Text {
                    anchors.verticalCenter: parent.verticalCenter

                    Layout.fillWidth: true

                    color: wrapper.ListView.isCurrentItem ? W.normalPalette.highlightedText : W.normalPalette.windowText

                    text: model.modelData.displayName
                }
            }

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    wrapper.ListView.view.currentIndex = index;
                }

                onDoubleClicked: {
                    root.accept();
                }
            }
        }
    }

    Rectangle {
        id: campaignMapListWrapper

        anchors {
            top: parent.top
            bottom: controls.top
            left: parent.left
            right: parent.right
            topMargin: 4
            bottomMargin: 4
        }

        color: W.normalPalette.window

        ScrollView {
            anchors.fill: parent

            ListView {
                id: campaignMapList

                currentIndex: -1

                model: root.campaignMaps.length ? root.campaignMaps : []

                delegate: campaignMapDelegate
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

        Item {
            Layout.fillWidth: true
        }

        Button {
            text: "Open"

            enabled: campaignMapList.currentIndex == -1 ? false : true
            onClicked: {
                root.accept();
            }
        }

        Button {
            text: "Cancel"
            onClicked: {
                root.reject();
            }
        }
    }

    onClosing: {
        root.reject();
    }

    function accept() {
        if(root.campaignMaps.length)
            root.campaignMap = root.campaignMaps[campaignMapList.currentIndex];
        else
            root.campaignMap = null;

        root.visible = false;
        root.accepted();
    }

    function reject() {
        root.campaignMap = null;

        root.visible = false;
        root.rejected();
    }
}
