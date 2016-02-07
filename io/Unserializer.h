#ifndef IO_UNSERIALIZER_H
#define IO_UNSERIALIZER_H

namespace warmonger {
namespace io {

namespace core {
    class Armor;
    class DamageType;
    class Faction;
    class Game;
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

class Unserializer
{
public:
    Unserializer = default;
    ~Unserializer = default;

    virtual core::Armor * unserialize(const QByteArray &data);
    virtual core::DamageType * unserialize(const QByteArray &data);
    virtual core::Faction * unserialize(const QByteArray &data);
    virtual core::Game * unserialize(const QByteArray &data);
    virtual core::Map * unserialize(const QByteArray &data);
    virtual core::MapNode * unserialize(const QByteArray &data);
    virtual core::Player * unserialize(const QByteArray &data);
    virtual core::Settlement * unserialize(const QByteArray &data);
    virtual core::SettlementType * unserialize(const QByteArray &data);
    virtual core::TerrainType * unserialize(const QByteArray &data);
    virtual core::Unit * unserialize(const QByteArray &data);
    virtual core::UnitClass * unserialize(const QByteArray &data);
    virtual core::UnitLevel * unserialize(const QByteArray &data);
    virtual core::UnitType * unserialize(const QByteArray &data);
    virtual core::Weapon * unserialize(const QByteArray &data);
    virtual core::World * unserialize(const QByteArray &data);
    virtual core::WorldSurface * unserialize(const QByteArray &data);
};

} // namespace warmonger
} // namespace io

#endif // IO_UNSERIALIZER_H
