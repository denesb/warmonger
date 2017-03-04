/** \file
 * World unserializer interface.
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

#ifndef IO_WORLD_UNSERIALIZER_H
#define IO_WORLD_UNSERIALIZER_H

#include <memory>

#include <QByteArray>

namespace warmonger {

namespace core {
class World;
class EntityType;
class ComponentType;
}

namespace io {

/**
 * World unserializer interface.
 *
 * Unserialize a world and related objects.
 */
class WorldUnserializer
{
public:
    virtual ~WorldUnserializer()
    {
    }

    /**
     * Unserialize a world.
     *
     * \param data the raw data
     *
     * \return the unserialized world object
     */
    virtual std::unique_ptr<core::World> unserializeWorld(const QByteArray& data) = 0;

    /**
     * Unserialize an entity-type.
     *
     * \param data the raw data
     *
     * \return the unserialized entity-type object
     */
    virtual std::unique_ptr<core::EntityType> unserializeEntityType(const QByteArray& data) = 0;

    /**
     * Unserialize a component-type.
     *
     * \param data the raw data
     *
     * \return the unserialized component-type object
     */
    virtual std::unique_ptr<core::ComponentType> unserializeComponentType(const QByteArray& data) = 0;
};

} // namespace warmonger
} // namespace io

#endif // IO_UNSERIALIZER_H
