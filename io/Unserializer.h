#ifndef IO_UNSERIALIZER_H
#define IO_UNSERIALIZER_H

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

class Unserializer
{
public:
    Unserializer()
    {
    }

    Unserializer(const Context& ctx)
        : ctx(ctx)
    {
    }

    Unserializer(const Context&& ctx)
        : ctx(std::move(ctx))
    {
    }

    virtual ~Unserializer()
    {
    }

    virtual core::Army* unserializeArmy(const QByteArray& data) = 0;
    virtual core::ArmyType* unserializeArmyType(const QByteArray& data) = 0;
    virtual core::Banner* unserializeBanner(const QByteArray& data) = 0;
    virtual core::CampaignMap* unserializeCampaignMap(const QByteArray& data) = 0;
    virtual core::Civilization* unserializeCivilization(const QByteArray& data) = 0;
    virtual core::Faction* unserializeFaction(const QByteArray& data) = 0;
    virtual core::MapNode* unserializeMapNode(const QByteArray& data) = 0;
    virtual core::Settlement* unserializeSettlement(const QByteArray& data) = 0;
    virtual core::SettlementType* unserializeSettlementType(const QByteArray& data) = 0;
    virtual core::TerrainType* unserializeTerrainType(const QByteArray& data) = 0;
    virtual core::Unit* unserializeUnit(const QByteArray& data) = 0;
    virtual core::UnitType* unserializeUnitType(const QByteArray& data) = 0;
    virtual core::World* unserializeWorld(const QByteArray& data) = 0;

protected:
    Context ctx;
};

} // namespace warmonger
} // namespace io

#endif // IO_UNSERIALIZER_H
