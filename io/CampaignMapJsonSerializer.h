/** \file
 * Campaign-map JSON Serializer.
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

#ifndef W_IO_CAMPAIGN_MAP_JSON_SERIALIZER_H
#define W_IO_CAMPAIGN_MAP_JSON_SERIALIZER_H

#include <QByteArray>
#include <QJsonDocument>

#include "io/CampaignMapSerializer.h"

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
 * Campaign-map json serializer.
 *
 * Serialize the campiang-map and related core game-objects.
 */
class CampaignMapJsonSerializer : public CampaignMapSerializer
{
public:
    /**
     * Construct a campaign-map json-serializer object.
     *
     * The compactness of the produced data can be changed by the format param.
     * Use QJsonDocument::Indented to produce nice human-readable output or
     * QJsonDocument::Compact to produce a compact output. The default is
     * QJsonDocument::Indented.
     *
     * \param format the formatting method to use
     */
    CampaignMapJsonSerializer(QJsonDocument::JsonFormat format = QJsonDocument::Indented);

    /**
     * Serialize an campaign-map to json.
     *
     * \param obj the campaign-map
     *
     * \return the raw serialized data
     */
    QByteArray serializeCampaignMap(const core::CampaignMap* const obj) const override;

    /**
     * Serialize a component to json.
     *
     * \param obj the component
     *
     * \return the raw serialized data
     */
    QByteArray serializeComponent(const core::Component* const obj) const override;

    /**
     * Serialize a entity json.
     *
     * \param obj the entity
     *
     * \return the raw serialized data
     */
    QByteArray serializeEntity(const core::Entity* const obj) const override;

    /**
     * Serialize an faction to json.
     *
     * \param obj the faction
     *
     * \return the raw serialized data
     */
    QByteArray serializeFaction(const core::Faction* const obj) const override;

    /**
     * Serialize an map-node to json.
     *
     * \param obj the map-node
     *
     * \return the raw serialized data
     */
    QByteArray serializeMapNode(const core::MapNode* const obj) const override;

private:
    QJsonDocument::JsonFormat format;
};

} // namespace warmonger
} // namespace io

#endif // W_IO_CAMPAIGN_MAP_JSON_SERIALIZER_H
