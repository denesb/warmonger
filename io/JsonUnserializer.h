/** \file
 * JSON unserializer.
 */

#ifndef IO_JSON_UNSERIALIZER_H
#define IO_JSON_UNSERIALIZER_H

#include "io/Unserializer.h"

namespace warmonger {
namespace io {

/**
 * JSON unserializer.
 *
 * Unserialize any core game-object from JSON.
 */
class JsonUnserializer : public Unserializer
{
public:
    using Unserializer::Unserializer;

    /**
     * Unserialize an army.
     *
     * \param data the raw data
     *
     * \return the unserialized army object
     */
    std::unique_ptr<core::Army> unserializeArmy(const QByteArray& data) override;

    /**
     * Unserialize an army-type.
     *
     * \param data the raw data
     *
     * \return the unserialized army-type object
     */
    std::unique_ptr<core::ArmyType> unserializeArmyType(const QByteArray& data) override;

    /**
     * Unserialize an banner.
     *
     * \param data the raw data
     *
     * \return the unserialized banner object
     */
    std::unique_ptr<core::Banner> unserializeBanner(const QByteArray& data) override;

    /**
     * Unserialize an campaign-map.
     *
     * \param data the raw data
     *
     * \return the unserialized campaign-map object
     */
    std::unique_ptr<core::CampaignMap> unserializeCampaignMap(const QByteArray& data) override;

    /**
     * Unserialize an civilization.
     *
     * \param data the raw data
     *
     * \return the unserialized civilization object
     */
    std::unique_ptr<core::Civilization> unserializeCivilization(const QByteArray& data) override;

    /**
     * Unserialize an faction.
     *
     * \param data the raw data
     *
     * \return the unserialized faction object
     */
    std::unique_ptr<core::Faction> unserializeFaction(const QByteArray& data) override;

    /**
     * Unserialize an map-node.
     *
     * \param data the raw data
     *
     * \return the unserialized map-node object
     */
    std::unique_ptr<core::MapNode> unserializeMapNode(const QByteArray& data) override;

    /**
     * Unserialize an settlement.
     *
     * \param data the raw data
     *
     * \return the unserialized settlement object
     */
    std::unique_ptr<core::Settlement> unserializeSettlement(const QByteArray& data) override;

    /**
     * Unserialize an settlement-type.
     *
     * \param data the raw data
     *
     * \return the unserialized settlement-type object
     */
    std::unique_ptr<core::SettlementType> unserializeSettlementType(const QByteArray& data) override;

    /**
     * Unserialize an terrain-type.
     *
     * \param data the raw data
     *
     * \return the unserialized terrain-type object
     */
    std::unique_ptr<core::TerrainType> unserializeTerrainType(const QByteArray& data) override;

    /**
     * Unserialize a unit.
     *
     * \param data the raw data
     *
     * \return the unserialized unit object
     */
    std::unique_ptr<core::Unit> unserializeUnit(const QByteArray& data) override;

    /**
     * Unserialize a unit-type.
     *
     * \param data the raw data
     *
     * \return the unserialized unit-type object
     */
    std::unique_ptr<core::UnitType> unserializeUnitType(const QByteArray& data) override;

    /**
     * Unserialize a world.
     *
     * \param data the raw data
     *
     * \return the unserialized world object
     */
    std::unique_ptr<core::World> unserializeWorld(const QByteArray& data) override;
};

} // namespace warmonger
} // namespace io

#endif // IO_JSON_UNSERIALIZER_H
