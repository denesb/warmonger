/** \file
 * Unserializer interface.
 */

#ifndef IO_UNSERIALIZER_H
#define IO_UNSERIALIZER_H

#include <memory>

#include "io/Context.h"

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
     * Unserialize an army.
     *
     * \param data the raw data
     *
     * \return the unserialized army object
     */
    virtual std::unique_ptr<core::Army> unserializeArmy(const QByteArray& data) = 0;

    /**
     * Unserialize an army-type.
     *
     * \param data the raw data
     *
     * \return the unserialized army-type object
     */
    virtual std::unique_ptr<core::ArmyType> unserializeArmyType(const QByteArray& data) = 0;

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
     * Unserialize an settlement.
     *
     * \param data the raw data
     *
     * \return the unserialized settlement object
     */
    virtual std::unique_ptr<core::Settlement> unserializeSettlement(const QByteArray& data) = 0;

    /**
     * Unserialize an settlement-type.
     *
     * \param data the raw data
     *
     * \return the unserialized settlement-type object
     */
    virtual std::unique_ptr<core::SettlementType> unserializeSettlementType(const QByteArray& data) = 0;

    /**
     * Unserialize an terrain-type.
     *
     * \param data the raw data
     *
     * \return the unserialized terrain-type object
     */
    virtual std::unique_ptr<core::TerrainType> unserializeTerrainType(const QByteArray& data) = 0;

    /**
     * Unserialize a unit.
     *
     * \param data the raw data
     *
     * \return the unserialized unit object
     */
    virtual std::unique_ptr<core::Unit> unserializeUnit(const QByteArray& data) = 0;

    /**
     * Unserialize a unit-type.
     *
     * \param data the raw data
     *
     * \return the unserialized unit-type object
     */
    virtual std::unique_ptr<core::UnitType> unserializeUnitType(const QByteArray& data) = 0;

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
