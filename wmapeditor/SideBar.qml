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
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import Warmonger 1.0

Rectangle {
    id: root

    property var miniMapWindowRect
    readonly property var miniMapWindowPos: miniMap.windowPos
    readonly property var objectType: objectTypeSelector.objectType
    property var availableObjectTypes
    property var editingMode

    color: W.normalPalette.window

    ObjectTypeSelector {
        id: objectTypeSelector

        anchors {
            top: parent.top
            bottom: miniMapWrapper.top
            left: parent.left
            right: parent.right
        }

        objectTypes: root.availableObjectTypes
    }

    Rectangle {
        id: miniMapWrapper

        height: 200
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        border {
            width: 1
            color: W.normalPalette.mid
        }

        color: W.normalPalette.window

        CampaignMiniMap {
            id: miniMap

            anchors {
                fill: parent
                margins: 1
            }

            campaignMap: W.campaignMap
            worldSurface: W.worldSurface
            windowRect: root.miniMapWindowRect
        }
    }
}
