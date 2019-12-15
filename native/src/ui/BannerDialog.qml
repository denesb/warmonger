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
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
import QtQuick.Window 2.2
import Warmonger 1.0

Window {
    id: root

    width: 500
    height: 200

    property var faction

    title: "Edit Banner"

    color: W.normalPalette.window

    function findIndex(list, item) {
        for (var i = 0; i < list.length; ++i) {
            if (list[i] == item)
                return i;
        }
        return -1;
    }

    Rectangle {

        anchors {
            fill: parent
            margins: 4
        }

        color: W.normalPalette.window

        Grid {
            id: grid

            anchors {
                top: parent.top
                bottom: parent.bottom
                left: parent.left
            }

            columns: 2
            spacing: 4
            verticalItemAlignment:  Grid.AlignVCenter

            Text {
                text: "Banner:"
            }

            ComboBox {
                model: W.world.banners
                textRole: "name"

                width: 200

                onActivated: {
                    root.faction.banner = W.world.banners[index];
                }
            }

            Text {
                text: "Primary Color:"
            }

            Rectangle {
                id: primaryColor

                width: 30
                height: 30

                radius: 4
                border {
                    width: 1
                    color: W.normalPalette.mid
                }

                color: W.color(root.faction.primaryColor)

                MouseArea {
                    anchors.fill: parent

                    onClicked: {
                        colorPicker.colorName = "primaryColor";

                        var pos = mapToItem(wrapper, x, y);
                        colorPicker.x = pos.x - 2;
                        colorPicker.y = pos.y - 2;
                        colorPicker.visible = true;
                    }
                }
            }

            Text {
                text: "Secondary Color:"
            }

            Rectangle {
                id: secondaryColor

                width: 30
                height: 30

                radius: 4
                border {
                    width: 1
                    color: W.normalPalette.mid
                }

                color: W.color(root.faction.secondaryColor)

                MouseArea {
                    anchors.fill: parent

                    onClicked: {
                        colorPicker.colorName = "secondaryColor";

                        var pos = mapToItem(wrapper, x, y);
                        colorPicker.x = pos.x - 2;
                        colorPicker.y = pos.y - 2;
                        colorPicker.visible = true;
                    }
                }
            }
        }

        Rectangle {
            id: bannerDisplay
            anchors {
                top: parent.top
                bottom: parent.bottom
                left: grid.right
                right: parent.right
            }

            color: W.normalPalette.window

            Banner {
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    verticalCenter: parent.vericalCenter
                }

                width: 120
                height: width

                banner: root.faction.banner
                primaryColor: root.faction.primaryColor
                secondaryColor: root.faction.secondaryColor
                worldSurface: W.worldSurface
            }
        }
    }

    Component {
        id: colorDelegate

        Item {
            width: 32
            height: 32

            Rectangle {
                width: 30
                height: 30

                anchors.centerIn: parent

                radius: 4
                border {
                    width: 1
                    color: W.normalPalette.mid
                }

                color: W.color(model.modelData)

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if (colorPicker.colorName == "primaryColor")
                            root.faction.primaryColor = model.modelData;
                        else
                            root.faction.secondaryColor = model.modelData;

                        colorPicker.visible = false;
                    }
                }
            }
        }
    }

    Item {
        id: wrapper

        anchors.fill: parent

        MouseArea {
            id: backgroundMouseArea

            anchors.fill: parent

            enabled: false

            onClicked: {
                colorPicker.visible = false;
            }
        }

        Rectangle {
            id: colorPicker

            property string colorName

            visible: false

            radius: 4
            border {
                width: 1
                color: W.normalPalette.mid
            }

            width: 228
            height: Math.ceil(W.world.colors.length / 7) * 32 + 4

            color: W.normalPalette.window

            GridView {
                id: colorsView

                anchors {
                    fill: parent
                    margins: 2
                }

                cellWidth: 32
                cellHeight: 32

                model: W.world.colors
                delegate: colorDelegate
            }

            onVisibleChanged: {
                backgroundMouseArea.enabled = visible;
            }
        }
    }
}
