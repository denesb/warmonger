/** \file
 * Campaign-map Serializer interface.
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

#ifndef W_IO_CAMPAIGN_MAP_SERIALIZER_H
#define W_IO_CAMPAIGN_MAP_SERIALIZER_H

#include <QByteArray>

namespace warmonger {

namespace core {
class CampaignMap;
class Component;
class Entity;
class Faction;
class MapNode;
}

namespace io {

/**
 * Campaign-map serializer interface.
 *
 * Serialize the campiang-map and related core game-objects.
 */
class CampaignMapSerializer
{
public:
    /**
     * Serialize an campaign-map.
     *
     * \param obj the campaign-map
     *
     * \return the raw serialized data
     */
    virtual QByteArray serializeCampaignMap(const core::CampaignMap* const obj) const = 0;

    /**
     * Serialize a component.
     *
     * \param obj the component
     *
     * \return the raw serialized data
     */
    virtual QByteArray serializeComponent(const core::Component* const obj) const = 0;

    /**
     * Serialize a entity.
     *
     * \param obj the entity
     *
     * \return the raw serialized data
     */
    virtual QByteArray serializeEntity(const core::Entity* const obj) const = 0;

    /**
     * Serialize an faction.
     *
     * \param obj the faction
     *
     * \return the raw serialized data
     */
    virtual QByteArray serializeFaction(const core::Faction* const obj) const = 0;

    /**
     * Serialize an map-node.
     *
     * \param obj the map-node
     *
     * \return the raw serialized data
     */
    virtual QByteArray serializeMapNode(const core::MapNode* const obj) const = 0;
};

} // namespace warmonger
} // namespace io

#endif // W_IO_CAMPAIGN_MAP_SERIALIZER_H
