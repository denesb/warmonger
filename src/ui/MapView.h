/** \file
 * MapView class.
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

#ifndef W_UI_MAP_PREVIEW_H
#define W_UI_MAP_PREVIEW_H

#include <QMatrix4x4>
#include <QtQuick/QQuickItem>

#include "core/Map.h"
#include "ui/BasicMap.h"
#include "ui/WorldSurface.h"

namespace warmonger {
namespace ui {

class MapWatcher;

/**
 * Presents a non-interactive preview of the campaign-map.
 *
 * For the map to be actually drawn the MapView needs a campaign-map
 * and a matching, actvivated  world-surface (that belongs to the same world)
 * set. When these conditions are not met the QQuickItem::ItemHasContents flag
 * is unset and nothing is going to be drawn on the screen.
 *
 * \see MapView::setMap
 * \see MapView::setWorldSurface
 * \see core::Map
 * \see WorldSurface
 */
class MapView : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(warmonger::core::Map* map READ getMap WRITE setMap NOTIFY mapChanged)
    Q_PROPERTY(WorldSurface* worldSurface READ getWorldSurface WRITE setWorldSurface NOTIFY worldSurfaceChanged)

public:
    /**
     * Constructs an empty campaign-map preview.
     */
    MapView(QQuickItem* parent = nullptr);

    /**
     * Get the shown campaign-map
     *
     * The MapView does not own the campaign-map!
     *
     * \return the campaign-map
     */
    core::Map* getMap() const
    {
        return this->map;
    }

    /**
     * Set the campaign map to show.
     *
     * If all conditions are given for drawing the map, this will trigger a
     * redraw. If this was the missing piece it will trigger the first drawing.
     * The MapView does not assume ownership of the campaign-map!
     * Will emit the signal MapView::mapChanged() if the newly
     * set value is different than the current one.
     *
     * \param map the campaign-map
     */
    void setMap(core::Map* map);

    /**
     * Get the world-surface used for drawing the campign-map.
     *
     * The MapView does not own the world-surface!
     *
     * \return the world-surface
     */
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
     * Will emit the signal MapView::worldSurfaceChanged() if the
     * newly set value is different than the current one.
     *
     * \param worldSurface the world-surface
     */
    void setWorldSurface(WorldSurface* worldSurface);

    /**
     * Update the scene-graph.
     *
     * \see QQuickItem::updatePaintNode()
     */
    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* oldNodeData) override;

signals:
    /**
     * Emitted when the map-node changes.
     */
    void mapChanged();

    /**
     * Emitted when the world-surface changes.
     */
    void worldSurfaceChanged();

private:
    void updateContent();
    void updateMapRect();
    void onMapNodesChanged();
    void updateTransform();

    QRect mapRect;
    QMatrix4x4 transform;

    core::Map* map;
    WorldSurface* worldSurface;
    std::map<core::MapNode*, QPoint> mapNodesPos;

    MapWatcher* watcher;
};

} // namespace ui
} // namespace warmonger

#endif // W_UI_MAP_PREVIEW_H
