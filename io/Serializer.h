#ifndef IO_SERIALIZER_H
#define IO_SERIALIZER_H

#include <QByteArray>

namespace warmonger {

namespace core {
    class Army;
    class ArmyType;
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

class Serializer
{
public:
    virtual QByteArray serializeArmy(const core::Army *obj) = 0;
    virtual QByteArray serializeArmyType(const core::ArmyType *obj) = 0;
    virtual QByteArray serializeCampaignMap(const core::CampaignMap *obj) = 0;
    virtual QByteArray serializeCivilization(const core::Civilization *obj) = 0;
    virtual QByteArray serializeFaction(const core::Faction *obj) = 0;
    virtual QByteArray serializeMapNode(const core::MapNode *obj) = 0;
    virtual QByteArray serializeSettlement(const core::Settlement *obj) = 0;
    virtual QByteArray serializeSettlementType(const core::SettlementType *obj) = 0;
    virtual QByteArray serializeTerrainType(const core::TerrainType *obj) = 0;
    virtual QByteArray serializeUnit(const core::Unit *obj) = 0;
    virtual QByteArray serializeUnitType(const core::UnitType *obj) = 0;
    virtual QByteArray serializeWorld(const core::World *obj) = 0;
};

} // namespace warmonger
} // namespace io

#endif // IO_SERIALIZER_H
