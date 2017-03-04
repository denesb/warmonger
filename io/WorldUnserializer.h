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
#include <vector>

#include <QByteArray>

namespace warmonger {

namespace core {
class Banner;
class Civilization;
class ComponentType;
class EntityType;
class World;
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
     * Unserialize an banner.
     *
     * \param data the raw data
     *
     * \return the unserialized banner object
     */
    virtual std::unique_ptr<core::Banner> unserializeBanner(
        const QByteArray& data, const std::vector<core::Civilization*>& allCivilizations) const = 0;

    /**
     * Unserialize an civilization.
     *
     * \param data the raw data
     *
     * \return the unserialized civilization object
     */
    virtual std::unique_ptr<core::Civilization> unserializeCivilization(const QByteArray& data) const = 0;

    /**
     * Unserialize a component-type.
     *
     * \param data the raw data
     *
     * \return the unserialized component-type object
     */
    virtual std::unique_ptr<core::ComponentType> unserializeComponentType(const QByteArray& data) const = 0;

    /**
     * Unserialize an entity-type.
     *
     * \param data the raw data
     *
     * \return the unserialized entity-type object
     */
    virtual std::unique_ptr<core::EntityType> unserializeEntityType(
        const QByteArray& data, const std::vector<core::ComponentType*>& allComponentTypes) const = 0;

    /**
     * Unserialize a world.
     *
     * \param data the raw data
     *
     * \return the unserialized world object
     */
    virtual std::unique_ptr<core::World> unserializeWorld(const QByteArray& data) const = 0;
};

} // namespace warmonger
} // namespace io

#endif // IO_UNSERIALIZER_H
