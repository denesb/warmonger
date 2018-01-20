/** \file
 * MiniMap class.
 *
 * \copyright (C) 2015-2018 Botond Dénes
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

#ifndef W_UI_CAMPAIGN_MINI_MAP_H
#define W_UI_CAMPAIGN_MINI_MAP_H

#include <unordered_map>

#include "core/Map.h"
#include "ui/BasicMiniMap.h"
#include "ui/WorldSurface.h"

namespace warmonger {

namespace core {
class MapNode;
class World;
}

namespace ui {

class MapWatcher;

class MiniMap : public BasicMiniMap
{
    Q_OBJECT

    Q_PROPERTY(warmonger::core::Map* map READ getMap WRITE setMap NOTIFY mapChanged)
    Q_PROPERTY(
        warmonger::ui::WorldSurface* worldSurface READ getWorldSurface WRITE setWorldSurface NOTIFY worldSurfaceChanged)
public:
    MiniMap(QQuickItem* parent = nullptr);

    core::Map* getMap() const;
    void setMap(core::Map* map);

    WorldSurface* getWorldSurface() const;
    void setWorldSurface(WorldSurface* worldSurface);

    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* data) override;

signals:
    void mapChanged();
    void worldSurfaceChanged();
    void windowRectChanged();

private:
    void updateContent();
    void updateMapRect();
    void onMapNodesChanged();

    WorldSurface* worldSurface;
    core::Map* map;
    std::unordered_map<core::MapNode*, QPoint> mapNodesPos;

    MapWatcher* watcher;
};

} // namespace ui
} // namespace warmonger

#endif // W_UI_CAMPAIGN_MINI_MAP_H
