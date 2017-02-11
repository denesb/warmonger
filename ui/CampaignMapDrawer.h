/** \file
 * CampaignMapDrawer class.
 *
 * \copyright (C) 2015-2017 Botond Dénes
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

#ifndef W_UI_CAMPAIGN_MAP_DRAWER_H
#define W_UI_CAMPAIGN_MAP_DRAWER_H

class QSGNode;

#include "core/CampaignMap.h"

namespace warmonger {
namespace ui {

class CampaignMapDrawer
{
public:
    /**
     * Draw a map-node and all content on it
     * @param[in] core::MapNode* mapNode the map-node to draw
     * @param[in] core::Settlement* settlement the settlement on the map-node
     * or nullptr
     * @param[in] core::Army* army the army on the map-node or nullptr
     * @param[in] oldNode a QSGNode that can be reused while drawing, if this
     * is not nullptr try to make as few modifications as possible.
     * @returns QSGNode* the result scene-graph node
     */
    virtual QSGNode* drawContent(const core::CampaignMap::Content& content, QSGNode* oldNode) = 0;
};

} // namespace ui
} // namespace warmonger

#endif // W_UI_CAMPAIGN_MAP_DRAWER_H
