/** \file
 * Map JSON unserializer class.
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

#ifndef IO_MAP_JSON_UNSERIALIZER_H
#define IO_MAP_JSON_UNSERIALIZER_H

#include "io/MapUnserializer.h"

namespace warmonger {

namespace core {
class Map;
class Entity;
class Faction;
class MapNode;
}

namespace io {

/**
 * Campaign-map json unserializer.
 *
 * Unserialize the campaign-map and related core game-object from json.
 */
class MapJsonUnserializer : public MapUnserializer
{
public:
    std::unique_ptr<core::Map> unserializeMap(
        const QByteArray& data, core::World* world) const override;
    core::Entity* unserializeEntity(const QByteArray& data, core::Map* map) const override;
    core::Faction* unserializeFaction(const QByteArray& data, core::Map* map) const override;
    core::MapNode* unserializeMapNode(const QByteArray& data, core::Map* map) const override;
};

} // namespace warmonger
} // namespace io

#endif // IO_MAP_JSON_UNSERIALIZER_H
