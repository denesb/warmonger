/** \file
 * Serializer interface.
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

#ifndef IO_SERIALIZER_H
#define IO_SERIALIZER_H

#include <QByteArray>

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
 * Serializer interface.
 *
 * Serialize any core game-object.
 */
class Serializer
{
public:
    /**
     * Serialize an banner.
     *
     * \param obj the banner
     *
     * \return the raw serialized data
     */
    virtual QByteArray serializeBanner(const core::Banner* obj) = 0;

    /**
     * Serialize an campaign-map.
     *
     * \param obj the campaign-map
     *
     * \return the raw serialized data
     */
    virtual QByteArray serializeCampaignMap(const core::CampaignMap* obj) = 0;

    /**
     * Serialize an civilization.
     *
     * \param obj the civilization
     *
     * \return the raw serialized data
     */
    virtual QByteArray serializeCivilization(const core::Civilization* obj) = 0;

    /**
     * Serialize an faction.
     *
     * \param obj the faction
     *
     * \return the raw serialized data
     */
    virtual QByteArray serializeFaction(const core::Faction* obj) = 0;

    /**
     * Serialize an map-node.
     *
     * \param obj the map-node
     *
     * \return the raw serialized data
     */
    virtual QByteArray serializeMapNode(const core::MapNode* obj) = 0;

    /**
     * Serialize an world.
     *
     * \param obj the world
     *
     * \return the raw serialized data
     */
    virtual QByteArray serializeWorld(const core::World* obj) = 0;
};

} // namespace warmonger
} // namespace io

#endif // IO_SERIALIZER_H
