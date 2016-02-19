#include <QJsonDocument>
#include <QJsonObject>

#include "core/Armor.h"
#include "core/DamageType.h"
#include "core/TerrainType.h"
#include "core/UnitClass.h"
#include "core/UnitLevel.h"
#include "core/Weapon.h"
#include "io/JsonUnserializer.h"
#include "test/Util.h"

using namespace warmonger;
using namespace warmonger::io;

/**
 * Generate a callable to get Type from the context.
 * Only usable in an instance member function.
 */
#define typeFromContext(Type) \
    std::bind( \
        &Context::get<Type>, \
        &this->ctx, \
        std::placeholders::_1 \
    ) \

/**
 * Convert QJsonObject to Container.
 *
 * Container must be a mapping type supporting assigment via the []
 * operator. For the conversion a `convertKey` and a `convertValue`
 * function must be supplied, where `convertKey` converts from QString
 * to Container::key_type and `convertValue converts from QJsonValue to
 * Container::mapped_type.
 */
template<typename Container, typename ConvertKeyFunc, typename ConvertValueFunc>
Container fromQJsonObject(
    const QJsonObject &object,
    ConvertKeyFunc convertKey,
    ConvertValueFunc convertValue
)
{
    Container container;

    for(auto it = object.begin(); it != object.end(); it++)
    {
        typename Container::key_type key = convertKey(it.key());
        typename Container::mapped_type value = convertValue(it.value());

        container[key] = value;
    }

    return container;
}

/**
 * QJsonValue to int.
 */
inline int qJsonValueToInt(const QJsonValue &value)
{
    return value.toInt();
}

JsonUnserializer::JsonUnserializer(const Context &ctx) :
    Unserializer(ctx)
{
}

core::Armor * JsonUnserializer::unserializeArmor(
    const QByteArray &data
)
{
    QJsonDocument jdoc(QJsonDocument::fromJson(data));
    QJsonObject jobj = jdoc.object();

    core::Armor *obj = new core::Armor();
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setDefenses(fromQJsonObject<QMap<const core::DamageType *, int>>(
        jobj["defenses"].toObject(),
        typeFromContext(core::DamageType *),
        qJsonValueToInt
    ));

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
    QJsonDocument jdoc(QJsonDocument::fromJson(data));
    QJsonObject jobj = jdoc.object();

    core::TerrainType *obj = new core::TerrainType();
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());

    return obj;
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
    QJsonDocument jdoc(QJsonDocument::fromJson(data));
    QJsonObject jobj = jdoc.object();

    core::UnitClass *obj = new core::UnitClass();
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setMovementPoints(jobj["movementPoints"].toInt());
    obj->setMovementCosts(fromQJsonObject<QMap<const core::TerrainType *, int>>(
        jobj["movementCosts"].toObject(),
        typeFromContext(core::TerrainType *),
        qJsonValueToInt
    ));
    obj->setAttacks(fromQJsonObject<QMap<const core::TerrainType *, int>>(
        jobj["attacks"].toObject(),
        typeFromContext(core::TerrainType *),
        qJsonValueToInt
    ));
    obj->setDefenses(fromQJsonObject<QMap<const core::TerrainType *, int>>(
        jobj["defenses"].toObject(),
        typeFromContext(core::TerrainType *),
        qJsonValueToInt
    ));

    return obj;
}

core::UnitLevel * JsonUnserializer::unserializeUnitLevel(
    const QByteArray &data
)
{
    QJsonDocument jdoc(QJsonDocument::fromJson(data));
    QJsonObject jobj = jdoc.object();

    core::UnitLevel *obj = new core::UnitLevel();
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setExperiencePoints(jobj["experiencePoints"].toInt());
    obj->setIndex(jobj["index"].toInt());

    return obj;
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
    QJsonDocument jdoc(QJsonDocument::fromJson(data));
    QJsonObject jobj = jdoc.object();

    core::Weapon *obj = new core::Weapon();
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setRange(jobj["range"].toInt());
    obj->setDamages(fromQJsonObject<QMap<const core::DamageType *, int>>(
        jobj["damages"].toObject(),
        typeFromContext(core::DamageType *),
        qJsonValueToInt
    ));

    return obj;
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
