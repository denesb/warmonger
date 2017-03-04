/** \file
 * World JSON unserializer.
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

#ifndef IO_WORLD_JSON_UNSERIALIZER_H
#define IO_WORLD_JSON_UNSERIALIZER_H

#include "io/WorldUnserializer.h"

namespace warmonger {
namespace io {

/**
 * World JSON unserializer.
 *
 * Unserialize a world and related objects from JSON.
 */
class WorldJsonUnserializer : public WorldUnserializer
{
public:
    std::unique_ptr<core::World> unserializeWorld(const QByteArray& data) override;
    std::unique_ptr<core::EntityType> unserializeEntityType(const QByteArray& data) override;
    std::unique_ptr<core::ComponentType> unserializeComponentType(const QByteArray& data) override;
};

} // namespace warmonger
} // namespace io

#endif // IO_WORLD_JSON_UNSERIALIZER_H
