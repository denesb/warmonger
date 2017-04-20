/** \file
 * CampaignMap unserializer interface.
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

#ifndef IO_CAMPAIGNMAP_UNSERIALIZER_H
#define IO_CAMPAIGNMAP_UNSERIALIZER_H

#include <memory>

#include <QByteArray>

namespace warmonger {

namespace core {
class CampaignMap;
class Entity;
class Faction;
class MapNode;
class World;
}

namespace io {

/**
 * CampaignMapUnserializer interface.
 *
 * Unserialize the campaign-map and related core game-object.
 */
class CampaignMapUnserializer
{
public:
    /**
     * Destructor.
     */
    virtual ~CampaignMapUnserializer()
    {
    }

    /**
     * Unserialize a campaign-map.
     *
     * \param data the raw data
     * \param world the world the map belongs to
     *
     * \return the unserialized campaign-map object
     */
    virtual std::unique_ptr<core::CampaignMap> unserializeCampaignMap(
        const QByteArray& data, core::World* world) const = 0;

    /**
     * Unserialize the entity and add it to the map.
     *
     * \param data the raw data
     * \param map the map the entity belongs to
     *
     * \return the unserialized entity object
     */
    virtual core::Entity* unserializeEntity(const QByteArray& data, core::CampaignMap* map) const = 0;

    /**
     * Unserialize the faction and add it to the map.
     *
     * \param data the raw data
     * \param map the map the faction belongs to
     *
     * \return the unserialized faction object
     */
    virtual core::Faction* unserializeFaction(const QByteArray& data, core::CampaignMap* map) const = 0;

    /**
     * Unserialize the map-node and add it to the map.
     *
     * \param data the raw data
     * \param map the map the map-node belongs to
     *
     * \return the unserialized map-node object
     */
    virtual core::MapNode* unserializeMapNode(const QByteArray& data, core::CampaignMap* map) const = 0;
};

} // namespace warmonger
} // namespace io

#endif // IO_CAMPAIGNMAP_UNSERIALIZER_H
