/** \file
 * Map class.
 *
 * \copyright (C) 2015-2019 Botond Dénes
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

#ifndef W_UI_MAP_H
#define W_UI_MAP_H

#include <unordered_map>

#include "core/Map.h"
#include "ui/BasicMap.h"
#include "ui/WorldSurface.h"

namespace warmonger {
namespace ui {

class Map : public BasicMap
{
    Q_OBJECT
    Q_PROPERTY(warmonger::core::Map* map READ getMap WRITE setMap NOTIFY mapChanged)
    Q_PROPERTY(WorldSurface* worldSurface READ getWorldSurface WRITE setWorldSurface NOTIFY worldSurfaceChanged)
public:
    Map(QQuickItem* parent = nullptr);

    core::Map* getMap() const
    {
        return this->map;
    }

    /**
     * Set the map to show.
     *
     * If all conditions are given for drawing the map, this will trigger a
     * redraw. If this was the missing piece it will trigger the first drawing.
     * The MapView does not assume ownership of the campaign-map!
     * Will emit the signal MapView::mapChanged() if the newly
     * set value is different than the current one.
     */
    void setMap(core::Map* map);

    WorldSurface* getWorldSurface() const
    {
        return this->worldSurface;
    }

    /**
     * Set the world-surface used for drawing the map.
     *
     * If all conditions are given for drawing the map, this will trigger a
     * redraw. If this was the missing piece it will trigger the first drawing.
     * The MapView does not assume ownership of the world-surface!
     * Will emit the signal Map::worldSurfaceChanged() if the
     * newly set value is different than the current one.
     */
    void setWorldSurface(WorldSurface* worldSurface);

    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* oldNodeData) override;

signals:
    void mapChanged();
    void worldSurfaceChanged();

private:
    void maybeUpdateContent();

    core::Map* map = nullptr;
    WorldSurface* worldSurface = nullptr;

    std::unordered_map<core::MapNode*, QPoint> mapNodesPos;
    graphics::Map graphicMap;
};

} // namespace ui
} // namespace warmonger

#endif // W_UI_MAP_H