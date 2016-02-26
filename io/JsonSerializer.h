#ifndef IO_JSON_SERIALIZER_H
#define IO_JSON_SERIALIZER_H

#include <QJsonDocument>

#include "io/Serializer.h"

namespace warmonger {
namespace io {

class JsonSerializer :
    public Serializer
{
public:
    JsonSerializer(QJsonDocument::JsonFormat format = QJsonDocument::Indented);

    virtual QByteArray serializeArmor(const core::Armor *obj);
    virtual QByteArray serializeDamageType(const core::DamageType *obj);
    virtual QByteArray serializeFaction(const core::Faction *obj);
    virtual QByteArray serializeMap(const core::Map *obj);
    virtual QByteArray serializeMapNode(const core::MapNode *obj);
    virtual QByteArray serializePlayer(const core::Player *obj);
    virtual QByteArray serializeSettlement(const core::Settlement *obj);
    virtual QByteArray serializeSettlementType(const core::SettlementType *obj);
    virtual QByteArray serializeTerrainType(const core::TerrainType *obj);
    virtual QByteArray serializeUnit(const core::Unit *obj);
    virtual QByteArray serializeUnitClass(const core::UnitClass *obj);
    virtual QByteArray serializeUnitLevel(const core::UnitLevel *obj);
    virtual QByteArray serializeUnitType(const core::UnitType *obj);
    virtual QByteArray serializeWeapon(const core::Weapon *obj);
    virtual QByteArray serializeWorld(const core::World *obj);
    virtual QByteArray serializeWorldSurface(const core::WorldSurface *obj);

private:
    QJsonDocument::JsonFormat format;
};

} // namespace warmonger
} // namespace io

#endif // IO_JSON_SERIALIZER_H
