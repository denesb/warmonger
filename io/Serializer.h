#ifndef IO_SERIALIZER_H
#define IO_SERIALIZER_H

#include <QByteArray>

namespace warmonger {

namespace core {
    class Armor;
    class DamageType;
    class Faction;
    class Map;
    class MapNode;
    class Player;
    class Settlement;
    class SettlementType;
    class TerrainType;
    class Unit;
    class UnitClass;
    class UnitLevel;
    class UnitType;
    class Weapon;
    class World;
    class WorldSurface;
}

namespace io {

class Serializer
{
public:
    virtual QByteArray serialize(const core::Armor *obj) = 0;
    virtual QByteArray serialize(const core::DamageType *obj) = 0;
    virtual QByteArray serialize(const core::Faction *obj) = 0;
    virtual QByteArray serialize(const core::Map *obj) = 0;
    virtual QByteArray serialize(const core::MapNode *obj) = 0;
    virtual QByteArray serialize(const core::Player *obj) = 0;
    virtual QByteArray serialize(const core::Settlement *obj) = 0;
    virtual QByteArray serialize(const core::SettlementType *obj) = 0;
    virtual QByteArray serialize(const core::TerrainType *obj) = 0;
    virtual QByteArray serialize(const core::Unit *obj) = 0;
    virtual QByteArray serialize(const core::UnitClass *obj) = 0;
    virtual QByteArray serialize(const core::UnitLevel *obj) = 0;
    virtual QByteArray serialize(const core::UnitType *obj) = 0;
    virtual QByteArray serialize(const core::Weapon *obj) = 0;
    virtual QByteArray serialize(const core::World *obj) = 0;
    virtual QByteArray serialize(const core::WorldSurface *obj) = 0;
};

} // namespace warmonger
} // namespace io

#endif // IO_SERIALIZER_H
