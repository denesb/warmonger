#include <algorithm>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "core/Armor.h"
#include "core/DamageType.h"
#include "core/Faction.h"
#include "core/MapNode.h"
#include "core/Player.h"
#include "core/Settlement.h"
#include "core/SettlementType.h"
#include "core/TerrainType.h"
#include "core/Unit.h"
#include "core/UnitClass.h"
#include "core/UnitLevel.h"
#include "core/Weapon.h"
#include "io/JsonUnserializer.h"
#include "test/Util.h"

using namespace warmonger;
using namespace warmonger::io;

/**
 * Functor object that retrieves object from the ctx.
 */
template <typename Type>
class ReferenceResolver
{
public:
    ReferenceResolver(const Context &ctx) :
        ctx(ctx)
    {
    }

    Type operator()(const QString &n) const
    {
        return ctx.get<Type>(n);
    }

    Type operator()(const QJsonValue &v) const
    {
        return (*this)(v.toString());
    }

private:
    const Context &ctx;
};


/**
 * Convert QJsonArray to Container.
 *
 * Container must be an STL-compatible container having a push_back
 * method. The convertFunc function converts QJsonValue to
 * Container::value_type.
 */
template<typename Container, typename ConvertFunc>
Container fromQJsonArray(const QJsonArray &array, ConvertFunc convertFunc)
{
    Container container;

    std::transform(
        array.begin(),
        array.end(),
        std::back_inserter(container),
        convertFunc
    );

    return container;
}

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
        ReferenceResolver<core::DamageType *>(this->ctx),
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
    QJsonDocument jdoc(QJsonDocument::fromJson(data));
    QJsonObject jobj = jdoc.object();

    ReferenceResolver<core::UnitType *> unitTypeRefResolver(this->ctx);

    core::Faction *obj = new core::Faction();
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setUnitTypes(fromQJsonArray<QList<core::UnitType *>>(
        jobj["unitTypes"].toArray(),
        unitTypeRefResolver
    ));
    obj->setRecruits(
        fromQJsonObject<QMap<core::SettlementType *, QList<core::UnitType *>>>(
            jobj["recruits"].toObject(),
            ReferenceResolver<core::SettlementType *>(this->ctx),
            [&](const QJsonValue &v)
            {
                return fromQJsonArray<QList<core::UnitType *>>(
                    v.toArray(),
                    unitTypeRefResolver
                );
            }
        )
    );

    return obj;
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
    QJsonDocument jdoc(QJsonDocument::fromJson(data));
    QJsonObject jobj = jdoc.object();

    core::MapNode *obj = new core::MapNode();
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setTerrainType(
        this->ctx.get<core::TerrainType *>(jobj["terrainType"].toString())
    );

    return obj;
}

core::Player * JsonUnserializer::unserializePlayer(
    const QByteArray &data
)
{
    QJsonDocument jdoc(QJsonDocument::fromJson(data));
    QJsonObject jobj = jdoc.object();

    core::Player *obj = new core::Player();
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setColor(QColor(jobj["color"].toString()));
    obj->setGoldBalance(jobj["goldBalance"].toInt());
    obj->setFaction(
        this->ctx.get<core::Faction *>(jobj["faction"].toString())
    );

    return obj;
}

core::Settlement * JsonUnserializer::unserializeSettlement(
    const QByteArray &data
)
{
    QJsonDocument jdoc(QJsonDocument::fromJson(data));
    QJsonObject jobj = jdoc.object();

    core::Settlement *obj = new core::Settlement();
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setType(
        this->ctx.get<core::SettlementType *>(jobj["type"].toString())
    );
    obj->setMapNode(
        this->ctx.get<core::MapNode *>(jobj["mapNode"].toString())
    );
    obj->setOwner(
        this->ctx.get<core::Player *>(jobj["owner"].toString())
    );

    return obj;
}

core::SettlementType * JsonUnserializer::unserializeSettlementType(
    const QByteArray &data
)
{
    QJsonDocument jdoc(QJsonDocument::fromJson(data));
    QJsonObject jobj = jdoc.object();

    core::SettlementType *obj = new core::SettlementType();
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setGoldPerTurn(jobj["goldPerTurn"].toInt());
    obj->setRecruits(fromQJsonArray<QList<core::UnitType *>>(
        jobj["recruits"].toArray(),
        ReferenceResolver<core::UnitType *>(this->ctx)
    ));

    return obj;
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
    QJsonDocument jdoc(QJsonDocument::fromJson(data));
    QJsonObject jobj = jdoc.object();

    core::Unit *obj = new core::Unit();
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setType(
        this->ctx.get<core::UnitType *>(jobj["type"].toString())
    );
    obj->setMapNode(
        this->ctx.get<core::MapNode *>(jobj["mapNode"].toString())
    );
    obj->setOwner(
        this->ctx.get<core::Player *>(jobj["owner"].toString())
    );

    return obj;
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
        ReferenceResolver<core::TerrainType *>(this->ctx),
        qJsonValueToInt
    ));
    obj->setAttacks(fromQJsonObject<QMap<const core::TerrainType *, int>>(
        jobj["attacks"].toObject(),
        ReferenceResolver<core::TerrainType *>(this->ctx),
        qJsonValueToInt
    ));
    obj->setDefenses(fromQJsonObject<QMap<const core::TerrainType *, int>>(
        jobj["defenses"].toObject(),
        ReferenceResolver<core::TerrainType *>(this->ctx),
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
    QJsonDocument jdoc(QJsonDocument::fromJson(data));
    QJsonObject jobj = jdoc.object();

    core::UnitType *obj = new core::UnitType();
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setClass(this->ctx.get<core::UnitClass *>(jobj["class"].toString()));
    obj->setLevel(this->ctx.get<core::UnitLevel *>(jobj["level"].toString()));
    obj->setHitPoints(jobj["hitPoints"].toInt());
    obj->setRecruitmentCost(jobj["recruitmentCost"].toInt());
    obj->setUpkeepCost(jobj["upkeepCost"].toInt());
    obj->setArmor(this->ctx.get<core::Armor *>(jobj["armor"].toString()));
    obj->setWeapons(fromQJsonArray<QList<core::Weapon *>>(
        jobj["weapons"].toArray(),
        ReferenceResolver<core::Weapon *>(this->ctx)
    ));
    obj->setUpgrades(fromQJsonArray<QList<core::UnitType *>>(
        jobj["upgrades"].toArray(),
        ReferenceResolver<core::UnitType *>(this->ctx)
    ));

    return obj;
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
        ReferenceResolver<core::DamageType *>(this->ctx),
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
