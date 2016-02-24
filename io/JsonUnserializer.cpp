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
#include "core/World.h"
#include "core/WorldSurface.h"
#include "io/Exception.h"
#include "io/JsonUnserializer.h"

#include <iostream>

using namespace warmonger;
using namespace warmonger::io;

core::Armor * armorFromJson(const QJsonObject &jobj, const Context &ctx);
core::DamageType * damageTypeFromJson(const QJsonObject &jobj);
core::Faction * factionsFromJson(const QJsonObject &jobj, const Context &ctx);
core::SettlementType * settlementTypeFromJson(
    const QJsonObject &jobj,
    const Context &ctx
);
core::TerrainType * terrainTypeFromJson(const QJsonObject &jobj);
core::UnitClass * unitClassFromJson(
    const QJsonObject &jobj,
    const Context &ctx
);
core::UnitLevel * unitLevelFromJson(const QJsonObject &jobj);
core::UnitType * unitTypeFromJson(const QJsonObject &jobj, const Context &ctx);
core::Weapon * weaponFromJson(const QJsonObject &jobj, const Context &ctx);

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
        Type o = ctx.get<Type>(n);
        if (o == nullptr)
        {
            typedef typename std::remove_pointer<Type>::type T;
            QString cn(T::staticMetaObject.className());
            throw UnresolvedReferenceError(
                QString("Unable to resolve reference %1 to %2")
                    .arg(n)
                    .arg(cn)
            );
        }

        return o;
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

template <class T>
QList<T *> objectListFromJson(
    const QJsonArray &array,
    Context &ctx,
    std::function<T * (const QJsonObject &)> fromJsonFunc
)
{
    QList<T *> objects = fromQJsonArray<QList<T *>>(
        array,
        [&](const QJsonValue &v)
        {
            return fromJsonFunc(v.toObject());
        }
    );

    std::for_each(
        objects.begin(),
        objects.end(),
        std::bind(&Context::add<T *>, &ctx, std::placeholders::_1)
    );

    return objects;
}

template <class T>
QList<T *> objectListFromJson(
    const QJsonArray &array,
    Context &ctx,
    std::function<T * (const QJsonObject &, const Context &ctx)> fromJsonFunc
)
{
    std::function<T * (const QJsonObject &)> func = std::bind(
        fromJsonFunc,
        std::placeholders::_1,
        ctx
    );
    return objectListFromJson<T>(array, ctx, func);
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
    return armorFromJson(jdoc.object(), this->ctx);
}

core::DamageType * JsonUnserializer::unserializeDamageType(
    const QByteArray &data
)
{
    QJsonDocument jdoc(QJsonDocument::fromJson(data));
    return damageTypeFromJson(jdoc.object());
}

core::Faction * JsonUnserializer::unserializeFaction(
    const QByteArray &data
)
{
    QJsonDocument jdoc(QJsonDocument::fromJson(data));
    return factionsFromJson(jdoc.object(), this->ctx);
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
    return settlementTypeFromJson(jdoc.object(), this->ctx);
}

core::TerrainType * JsonUnserializer::unserializeTerrainType(
    const QByteArray &data
)
{
    QJsonDocument jdoc(QJsonDocument::fromJson(data));
    return terrainTypeFromJson(jdoc.object());
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
    return unitClassFromJson(jdoc.object(), this->ctx);
}

core::UnitLevel * JsonUnserializer::unserializeUnitLevel(
    const QByteArray &data
)
{
    QJsonDocument jdoc(QJsonDocument::fromJson(data));
    return unitLevelFromJson(jdoc.object());
}

core::UnitType * JsonUnserializer::unserializeUnitType(
    const QByteArray &data
)
{
    QJsonDocument jdoc(QJsonDocument::fromJson(data));
    return unitTypeFromJson(jdoc.object(), this->ctx);
}

core::Weapon * JsonUnserializer::unserializeWeapon(
    const QByteArray &data
)
{
    QJsonDocument jdoc(QJsonDocument::fromJson(data));
    return weaponFromJson(jdoc.object(), this->ctx);
}

core::World * JsonUnserializer::unserializeWorld(
    const QByteArray &data
)
{
    QJsonDocument jdoc(QJsonDocument::fromJson(data));
    QJsonObject jobj = jdoc.object();

    Context worldCtx;

    core::World *obj = new core::World();
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setDamageTypes(objectListFromJson<core::DamageType>(
        jobj["damageTypes"].toArray(),
        worldCtx,
        damageTypeFromJson
    ));
    obj->setArmors(objectListFromJson<core::Armor>(
        jobj["armors"].toArray(),
        worldCtx,
        armorFromJson
    ));
    obj->setWeapons(objectListFromJson<core::Weapon>(
        jobj["weapons"].toArray(),
        worldCtx,
        weaponFromJson
    ));
    obj->setTerrainTypes(objectListFromJson<core::TerrainType>(
        jobj["terrainTypes"].toArray(),
        worldCtx,
        terrainTypeFromJson
    ));
    obj->setUnitClasses(objectListFromJson<core::UnitClass>(
        jobj["unitClasses"].toArray(),
        worldCtx,
        unitClassFromJson
    ));
    obj->setUnitLevels(objectListFromJson<core::UnitLevel>(
        jobj["unitLevels"].toArray(),
        worldCtx,
        unitLevelFromJson
    ));
    // UniTypes refer to other UnitTypes so we need to add UnitTypes
    // to the context as soon as they are unserialized
    obj->setUnitTypes(fromQJsonArray<QList<core::UnitType *>>(
        jobj["unitTypes"].toArray(),
        [&](const QJsonValue &v)
        {
            core::UnitType *ut = unitTypeFromJson(v.toObject(), worldCtx);
            worldCtx.add(ut);
            return ut;
        }
    ));
    obj->setSettlementTypes(objectListFromJson<core::SettlementType>(
        jobj["settlementTypes"].toArray(),
        worldCtx,
        settlementTypeFromJson
    ));
    obj->setFactions(objectListFromJson<core::Faction>(
        jobj["factions"].toArray(),
        worldCtx,
        factionsFromJson
    ));

    return obj;
}

core::WorldSurface * JsonUnserializer::unserializeWorldSurface(
    const QByteArray &data
)
{
    QJsonDocument jdoc(QJsonDocument::fromJson(data));
    QJsonObject jobj = jdoc.object();

    core::WorldSurface *obj = new core::WorldSurface();
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setTileWidth(jobj["tileWidth"].toInt());
    obj->setTileHeight(jobj["tileHeight"].toInt());

    return obj;
}

core::Armor * armorFromJson(const QJsonObject &jobj, const Context &ctx)
{
    core::Armor *obj = new core::Armor();
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setDefenses(fromQJsonObject<QMap<const core::DamageType *, int>>(
        jobj["defenses"].toObject(),
        ReferenceResolver<core::DamageType *>(ctx),
        qJsonValueToInt
    ));

    return obj;
}

core::DamageType * damageTypeFromJson(const QJsonObject &jobj)
{
    core::DamageType *obj = new core::DamageType();
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());

    return obj;
}

core::Faction * factionsFromJson(const QJsonObject &jobj, const Context &ctx)
{
    ReferenceResolver<core::UnitType *> unitTypeRefResolver(ctx);

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
            ReferenceResolver<core::SettlementType *>(ctx),
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

core::SettlementType * settlementTypeFromJson(
    const QJsonObject &jobj,
    const Context &ctx
)
{
    core::SettlementType *obj = new core::SettlementType();
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setGoldPerTurn(jobj["goldPerTurn"].toInt());
    obj->setRecruits(fromQJsonArray<QList<core::UnitType *>>(
        jobj["recruits"].toArray(),
        ReferenceResolver<core::UnitType *>(ctx)
    ));

    return obj;
}

core::TerrainType * terrainTypeFromJson(const QJsonObject &jobj)
{
    core::TerrainType *obj = new core::TerrainType();
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());

    return obj;
}

core::UnitClass * unitClassFromJson(const QJsonObject &jobj, const Context &ctx)
{
    core::UnitClass *obj = new core::UnitClass();
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setMovementPoints(jobj["movementPoints"].toInt());
    obj->setMovementCosts(fromQJsonObject<QMap<const core::TerrainType *, int>>(
        jobj["movementCosts"].toObject(),
        ReferenceResolver<core::TerrainType *>(ctx),
        qJsonValueToInt
    ));
    obj->setAttacks(fromQJsonObject<QMap<const core::TerrainType *, int>>(
        jobj["attacks"].toObject(),
        ReferenceResolver<core::TerrainType *>(ctx),
        qJsonValueToInt
    ));
    obj->setDefenses(fromQJsonObject<QMap<const core::TerrainType *, int>>(
        jobj["defenses"].toObject(),
        ReferenceResolver<core::TerrainType *>(ctx),
        qJsonValueToInt
    ));

    return obj;
}

core::UnitLevel * unitLevelFromJson(const QJsonObject &jobj)
{
    core::UnitLevel *obj = new core::UnitLevel();
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setExperiencePoints(jobj["experiencePoints"].toInt());
    obj->setIndex(jobj["index"].toInt());

    return obj;
}

core::UnitType * unitTypeFromJson(const QJsonObject &jobj, const Context &ctx)
{
    core::UnitType *obj = new core::UnitType();
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setClass(ctx.get<core::UnitClass *>(jobj["class"].toString()));
    obj->setLevel(ctx.get<core::UnitLevel *>(jobj["level"].toString()));
    obj->setHitPoints(jobj["hitPoints"].toInt());
    obj->setRecruitmentCost(jobj["recruitmentCost"].toInt());
    obj->setUpkeepCost(jobj["upkeepCost"].toInt());
    obj->setArmor(ctx.get<core::Armor *>(jobj["armor"].toString()));
    obj->setWeapons(fromQJsonArray<QList<core::Weapon *>>(
        jobj["weapons"].toArray(),
        ReferenceResolver<core::Weapon *>(ctx)
    ));
    obj->setUpgrades(fromQJsonArray<QList<core::UnitType *>>(
        jobj["upgrades"].toArray(),
        ReferenceResolver<core::UnitType *>(ctx)
    ));

    return obj;
}

core::Weapon * weaponFromJson(const QJsonObject &jobj, const Context &ctx)
{
    core::Weapon *obj = new core::Weapon();
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setRange(jobj["range"].toInt());
    obj->setDamages(fromQJsonObject<QMap<const core::DamageType *, int>>(
        jobj["damages"].toObject(),
        ReferenceResolver<core::DamageType *>(ctx),
        qJsonValueToInt
    ));

    return obj;
}
