/** \file
 * World Serializer interface.
 *
 * \copyright (C) 2014-2016 Botond Dénes
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

#ifndef W_IO_WORLD_SERIALIZER_H
#define W_IO_WORLD_SERIALIZER_H

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
 * World Serializer interface.
 *
 * Serialize the world and related core game-object.
 */
class WorldSerializer
{
public:
    /**
     * Serialize an banner.
     *
     * \param obj the banner
     *
     * \return the raw serialized data
     */
    virtual QByteArray serializeBanner(const core::Banner* const obj) const = 0;

    /**
     * Serialize an civilization.
     *
     * \param obj the civilization
     *
     * \return the raw serialized data
     */
    virtual QByteArray serializeCivilization(const core::Civilization* const obj) const = 0;

    /**
     * Serialize an component-type.
     *
     * \param obj the component-type
     *
     * \return the raw serialized data
     */
    virtual QByteArray serializeComponentType(const core::ComponentType* const obj) const = 0;

    /**
     * Serialize an entity-type.
     *
     * \param obj the entity-type
     *
     * \return the raw serialized data
     */
    virtual QByteArray serializeEntityType(const core::EntityType* const obj) const = 0;

    /**
     * Serialize an world.
     *
     * \param obj the world
     *
     * \return the raw serialized data
     */
    virtual QByteArray serializeWorld(const core::World* const obj) const = 0;
};

} // namespace warmonger
} // namespace io

#endif // W_IO_WORLD_SERIALIZER_H
