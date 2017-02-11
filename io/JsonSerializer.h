/** \file
 * JSON serializer.
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

#ifndef IO_JSON_SERIALIZER_H
#define IO_JSON_SERIALIZER_H

#include <QJsonDocument>

#include "io/Serializer.h"

namespace warmonger {
namespace io {

/**
 * JSON serializer.
 *
 * Serialize any core game-object to JSON.
 */
class JsonSerializer : public Serializer
{
public:
    /**
     * Construct a json-serializer object.
     *
     * The compactness of the produced data can be changed by the format param.
     * Use QJsonDocument::Indented to produce nice human-readable output or
     * QJsonDocument::Compact to produce a compact output. The default is
     * QJsonDocument::Indented.
     *
     * \param format the formatting method to use
     */
    JsonSerializer(QJsonDocument::JsonFormat format = QJsonDocument::Indented);

    /**
     * Serialize an army.
     *
     * \param obj the army
     *
     * \return the raw serialized data
     */
    QByteArray serializeArmy(const core::Army* obj) override;

    /**
     * Serialize an army-type.
     *
     * \param obj the army-type
     *
     * \return the raw serialized data
     */
    QByteArray serializeArmyType(const core::ArmyType* obj) override;

    /**
     * Serialize an banner.
     *
     * \param obj the banner
     *
     * \return the raw serialized data
     */
    QByteArray serializeBanner(const core::Banner* obj) override;

    /**
     * Serialize an campaign-map.
     *
     * \param obj the campaign-map
     *
     * \return the raw serialized data
     */
    QByteArray serializeCampaignMap(const core::CampaignMap* obj) override;

    /**
     * Serialize an civilization.
     *
     * \param obj the civilization
     *
     * \return the raw serialized data
     */
    QByteArray serializeCivilization(const core::Civilization* obj) override;

    /**
     * Serialize an faction.
     *
     * \param obj the faction
     *
     * \return the raw serialized data
     */
    QByteArray serializeFaction(const core::Faction* obj) override;

    /**
     * Serialize an map-node.
     *
     * \param obj the map-node
     *
     * \return the raw serialized data
     */
    QByteArray serializeMapNode(const core::MapNode* obj) override;

    /**
     * Serialize an settlement.
     *
     * \param obj the settlement
     *
     * \return the raw serialized data
     */
    QByteArray serializeSettlement(const core::Settlement* obj) override;

    /**
     * Serialize an settlement-type.
     *
     * \param obj the settlement-type
     *
     * \return the raw serialized data
     */
    QByteArray serializeSettlementType(const core::SettlementType* obj) override;

    /**
     * Serialize an terrain-type.
     *
     * \param obj the terrain-type
     *
     * \return the raw serialized data
     */
    QByteArray serializeTerrainType(const core::TerrainType* obj) override;

    /**
     * Serialize an unit.
     *
     * \param obj the unit
     *
     * \return the raw serialized data
     */
    QByteArray serializeUnit(const core::Unit* obj) override;

    /**
     * Serialize an unit-type.
     *
     * \param obj the unit-type
     *
     * \return the raw serialized data
     */
    QByteArray serializeUnitType(const core::UnitType* obj) override;

    /**
     * Serialize an world.
     *
     * \param obj the world
     *
     * \return the raw serialized data
     */
    QByteArray serializeWorld(const core::World* obj) override;

private:
    QJsonDocument::JsonFormat format;
};

} // namespace warmonger
} // namespace io

#endif // IO_JSON_SERIALIZER_H
