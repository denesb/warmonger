/** \file
 * Unserializer interface.
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

#ifndef IO_UNSERIALIZER_H
#define IO_UNSERIALIZER_H

#include <memory>

#include "io/Context.h"

namespace warmonger {

namespace core {
class Banner;
class CampaignMap;
class Civilization;
class Faction;
class MapNode;
class World;
}

namespace io {

/**
 * Unserializer interface.
 *
 * Unserialize any core game-object.
 */
class Unserializer
{
public:
    /**
     * Construct an Unserializer object with no context.
     */
    Unserializer()
    {
    }

    /**
     * Construct an Unserializer object with context.
     *
     * The context will be used to resolve external references while
     * unserializing.
     *
     * \param ctx the context
     */
    Unserializer(const Context& ctx)
        : ctx(ctx)
    {
    }

    /**
     * Destructor.
     */
    virtual ~Unserializer()
    {
    }

    /**
     * Unserialize an banner.
     *
     * \param data the raw data
     *
     * \return the unserialized banner object
     */
    virtual std::unique_ptr<core::Banner> unserializeBanner(const QByteArray& data) = 0;

    /**
     * Unserialize an campaign-map.
     *
     * \param data the raw data
     *
     * \return the unserialized campaign-map object
     */
    virtual std::unique_ptr<core::CampaignMap> unserializeCampaignMap(const QByteArray& data) = 0;

    /**
     * Unserialize an civilization.
     *
     * \param data the raw data
     *
     * \return the unserialized civilization object
     */
    virtual std::unique_ptr<core::Civilization> unserializeCivilization(const QByteArray& data) = 0;

    /**
     * Unserialize an faction.
     *
     * \param data the raw data
     *
     * \return the unserialized faction object
     */
    virtual std::unique_ptr<core::Faction> unserializeFaction(const QByteArray& data) = 0;

    /**
     * Unserialize an map-node.
     *
     * \param data the raw data
     *
     * \return the unserialized map-node object
     */
    virtual std::unique_ptr<core::MapNode> unserializeMapNode(const QByteArray& data) = 0;

    /**
     * Unserialize a world.
     *
     * \param data the raw data
     *
     * \return the unserialized world object
     */
    virtual std::unique_ptr<core::World> unserializeWorld(const QByteArray& data) = 0;

protected:
    Context ctx;
};

} // namespace warmonger
} // namespace io

#endif // IO_UNSERIALIZER_H
