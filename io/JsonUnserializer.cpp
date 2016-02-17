#include <QJsonDocument>
#include <QJsonObject>

#include "core/Armor.h"
#include "core/DamageType.h"
#include "io/JsonUnserializer.h"
#include "test/Util.h"

using namespace warmonger;
using namespace warmonger::io;

core::Armor * JsonUnserializer::unserializeArmor(
    const QByteArray &data
)
{
    QJsonDocument jdoc(QJsonDocument::fromJson(data));
    QJsonObject jobj = jdoc.object();

    core::Armor *obj = new core::Armor();
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());

    return obj;
}

core::DamageType * JsonUnserializer::unserializeDamageType(
    const QByteArray &data
)
{
    QJsonDocument jdoc(QJsonDocument::fromJson(data));
    QJsonObject jobj = jdoc.object();

    core::DamageType *obj = new core::DamageType();
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());

    return obj;
}

core::Faction * JsonUnserializer::unserializeFaction(
    const QByteArray &data
)
{
}

core::Map * JsonUnserializer::unserializeMap(
    const QByteArray &data
)
{
}

core::MapNode * JsonUnserializer::unserializeMapNode(
    const QByteArray &data
)
{
}

core::Player * JsonUnserializer::unserializePlayer(
    const QByteArray &data
)
{
}

core::Settlement * JsonUnserializer::unserializeSettlement(
    const QByteArray &data
)
{
}

core::SettlementType * JsonUnserializer::unserializeSettlementType(
    const QByteArray &data
)
{
}

core::TerrainType * JsonUnserializer::unserializeTerrainType(
    const QByteArray &data
)
{
}

core::Unit * JsonUnserializer::unserializeUnit(
    const QByteArray &data
)
{
}

core::UnitClass * JsonUnserializer::unserializeUnitClass(
    const QByteArray &data
)
{
}

core::UnitLevel * JsonUnserializer::unserializeUnitLevel(
    const QByteArray &data
)
{
}

core::UnitType * JsonUnserializer::unserializeUnitType(
    const QByteArray &data
)
{
}

core::Weapon * JsonUnserializer::unserializeWeapon(
    const QByteArray &data
)
{
}

core::World * JsonUnserializer::unserializeWorld(
    const QByteArray &data
)
{
}

core::WorldSurface * JsonUnserializer::unserializeWorldSurface(
    const QByteArray &data
)
{
}
