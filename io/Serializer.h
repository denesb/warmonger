/** \file
 * Serializer interface.
 */

#ifndef IO_SERIALIZER_H
#define IO_SERIALIZER_H

#include <QByteArray>

namespace warmonger {

namespace core {
class Army;
class ArmyType;
class Banner;
class CampaignMap;
class Civilization;
class Faction;
class MapNode;
class Settlement;
class SettlementType;
class TerrainType;
class Unit;
class UnitType;
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
     * Serialize an army.
     *
     * \param obj the army
     *
     * \return the raw serialized data
     */
    virtual QByteArray serializeArmy(const core::Army* obj) = 0;

    /**
     * Serialize an army-type.
     *
     * \param obj the army-type
     *
     * \return the raw serialized data
     */
    virtual QByteArray serializeArmyType(const core::ArmyType* obj) = 0;

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
     * Serialize an settlement.
     *
     * \param obj the settlement
     *
     * \return the raw serialized data
     */
    virtual QByteArray serializeSettlement(const core::Settlement* obj) = 0;

    /**
     * Serialize an settlement-type.
     *
     * \param obj the settlement-type
     *
     * \return the raw serialized data
     */
    virtual QByteArray serializeSettlementType(const core::SettlementType* obj) = 0;

    /**
     * Serialize an terrain-type.
     *
     * \param obj the terrain-type
     *
     * \return the raw serialized data
     */
    virtual QByteArray serializeTerrainType(const core::TerrainType* obj) = 0;

    /**
     * Serialize an unit.
     *
     * \param obj the unit
     *
     * \return the raw serialized data
     */
    virtual QByteArray serializeUnit(const core::Unit* obj) = 0;

    /**
     * Serialize an unit-type.
     *
     * \param obj the unit-type
     *
     * \return the raw serialized data
     */
    virtual QByteArray serializeUnitType(const core::UnitType* obj) = 0;

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
