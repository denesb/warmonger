#ifndef IO_JSON_UNSERIALIZER_H
#define IO_JSON_UNSERIALIZER_H

#include "io/Unserializer.h"

namespace warmonger {
namespace io {

class JsonUnserializer :
    public Unserializer
{
public:
    core::Armor * unserializeArmor(const QByteArray &data);
    core::DamageType * unserializeDamageType(const QByteArray &data);
    core::Faction * unserializeFaction(const QByteArray &data);
    core::Map * unserializeMap(const QByteArray &data);
    core::MapNode * unserializeMapNode(const QByteArray &data);
    core::Player * unserializePlayer(const QByteArray &data);
    core::Settlement * unserializeSettlement(const QByteArray &data);
    core::SettlementType * unserializeSettlementType(const QByteArray &data);
    core::TerrainType * unserializeTerrainType(const QByteArray &data);
    core::Unit * unserializeUnit(const QByteArray &data);
    core::UnitClass * unserializeUnitClass(const QByteArray &data);
    core::UnitLevel * unserializeUnitLevel(const QByteArray &data);
    core::UnitType * unserializeUnitType(const QByteArray &data);
    core::Weapon * unserializeWeapon(const QByteArray &data);
    core::World * unserializeWorld(const QByteArray &data);
    core::WorldSurface * unserializeWorldSurface(const QByteArray &data);
};

} // namespace warmonger
} // namespace io

#endif // IO_JSON_UNSERIALIZER_H
