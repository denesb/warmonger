/** \file
 * Render.
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
#ifndef W_UI_RENDER_H
#define W_UI_RENDER_H

#include <unordered_map>
#include <vector>

#include <QPoint>
#include <QRect>

class QSGNode;
class QQuickWindow;

namespace warmonger {

namespace core {
class MapNode;
class Entity;
}

namespace ui {

class WorldSurface;

struct RenderContext
{
    const WorldSurface* const surface;
    QQuickWindow* window;
    const std::unordered_map<core::MapNode*, QPoint>& mapNodesPos;
    QRect renderWindow;
};

QSGNode* renderEntities(const std::vector<core::Entity*>& entities, QSGNode* oldNode, const RenderContext& ctx);

} // namespace ui
} // namespace warmonger

#endif // W_UI_RENDER_H
