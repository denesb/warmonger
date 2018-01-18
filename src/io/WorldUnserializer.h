/** \file
 * World unserializer interface.
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

#ifndef IO_WORLD_UNSERIALIZER_H
#define IO_WORLD_UNSERIALIZER_H

#include <memory>
#include <vector>

#include <QByteArray>

namespace warmonger {

namespace core {
class Banner;
class Civilization;
class WorldComponentType;
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
     * Unserialize the banner and add it to the world.
     *
     * \param data the raw data
     * \param world the world
     *
     * \return the unserialized banner object
     */
    virtual core::Banner* unserializeBanner(const QByteArray& data, core::World* world) const = 0;

    /**
     * Unserialize the civilization and add it to the world.
     *
     * \param data the raw data
     * \param world the world
     *
     * \return the unserialized civilization object
     */
    virtual core::Civilization* unserializeCivilization(const QByteArray& data, core::World* world) const = 0;

    /**
     * Unserialize the world component-type and add it to the world.
     *
     * \param data the raw data
     * \param world the world
     *
     * \return the unserialized world component-type object
     */
    virtual core::WorldComponentType* unserializeWorldComponentType(
        const QByteArray& data, core::World* world) const = 0;

    /**
     * Unserialize the world.
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
