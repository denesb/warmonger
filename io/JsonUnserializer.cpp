#include <algorithm>
#include <functional>
#include <memory>
#include <vector>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "core/Armor.h"
#include "core/CampaignMap.h"
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

using namespace warmonger;
using namespace warmonger::io;

QJsonDocument parseJson(const QByteArray &json);

core::Armor * armorFromJson(const QJsonObject &jobj, Context &ctx);
core::DamageType * damageTypeFromJson(const QJsonObject &jobj);
core::Faction * factionsFromJson(const QJsonObject &jobj, Context &ctx);
core::MapNode * mapNodeFromJson(const QJsonObject &jobj, Context &ctx);
std::vector<core::MapNode *> mapNodesFromJson(const QJsonArray &jarr, Context &ctx);
core::Player * playerFromJson(const QJsonObject &jobj, Context &ctx);
core::Settlement * settlementFromJson(const QJsonObject &jobj, Context &ctx);
core::SettlementType * settlementTypeFromJson(const QJsonObject &jobj, Context &ctx);
core::TerrainType * terrainTypeFromJson(const QJsonObject &jobj);
core::Unit * unitFromJson(const QJsonObject &jobj, Context &ctx);
core::UnitClass * unitClassFromJson(const QJsonObject &jobj, Context &ctx);
core::UnitLevel * unitLevelFromJson(const QJsonObject &jobj);
core::UnitType * unitTypeFromJson(const QJsonObject &jobj, Context &ctx);
core::Weapon * weaponFromJson(const QJsonObject &jobj, Context &ctx);

/**
 * Resolve reference `name` to `Type`.
 */
template <typename Type>
Type * resolveReference(Context &ctx, const QString &name)
{
    Type *obj = ctx.get<Type *>(name);
    if (obj == nullptr)
    {
        QString className(Type::staticMetaObject.className());
        throw UnresolvedReferenceError(
            QString("Unable to resolve reference %1 to %2")
                .arg(name)
                .arg(className)
        );
    }

    return obj;
}

/**
 * Functor object that retrieves object from the ctx.
 */
template <typename Type>
class ReferenceResolver
{
public:
    ReferenceResolver(Context &ctx) :
        ctx(ctx)
    {
    }

    Type * operator()(const QString &n) const
    {
        return resolveReference<Type>(this->ctx, n);
    }

    Type * operator()(const QJsonValue &v) const
    {
        return resolveReference<Type>(this->ctx, v.toString());
    }

private:
    Context &ctx;
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
std::vector<T *> objectListFromJson(
    const QJsonArray &array,
    Context &ctx,
    std::function<T * (const QJsonObject &)> fromJsonFunc
)
{
    std::vector<T *> objects = fromQJsonArray<std::vector<T *>>(
        array,
        [&](const QJsonValue &v)
        {
            return fromJsonFunc(v.toObject());
        }
    );

    std::for_each(
        objects.begin(),
        objects.end(),
        std::bind(&Context::add, &ctx, std::placeholders::_1)
    );

    return objects;
}

template <class T>
std::vector<T *> objectListFromJson(
    const QJsonArray &array,
    Context &ctx,
    std::function<T * (const QJsonObject &, Context &ctx)> fromJsonFunc
)
{
    std::function<T * (const QJsonObject &)> func = std::bind(
        fromJsonFunc,
        std::placeholders::_1,
        ctx
    );
    return objectListFromJson<T>(array, ctx, func);
}

JsonUnserializer::JsonUnserializer(Context &ctx) :
    Unserializer(ctx)
{
}

core::Armor * JsonUnserializer::unserializeArmor(
    const QByteArray &data
)
{
    QJsonDocument jdoc(parseJson(data));
    return armorFromJson(jdoc.object(), this->ctx);
}

core::CampaignMap * JsonUnserializer::unserializeCampaignMap(const QByteArray &data)
{
    QJsonDocument jdoc(parseJson(data));
    QJsonObject jobj = jdoc.object();

    std::unique_ptr<core::CampaignMap> obj(new core::CampaignMap());

    obj->setObjectName(jobj["objectName"].toString());

    obj->setDisplayName(jobj["displayName"].toString());

    obj->setWorld(
        resolveReference<core::World>(this->ctx, jobj["world"].toString())
    );

    obj->setMapNodeIndex(jobj["mapNodeIndex"].toInt());

    obj->setSettlementIndex(jobj["settlementIndex"].toInt());

    obj->setUnitIndex(jobj["unitIndex"].toInt());

    obj->setMapNodes(mapNodesFromJson(jobj["mapNodes"].toArray(), this->ctx));

    obj->setPlayers(objectListFromJson<core::Player>(
        jobj["players"].toArray(),
        this->ctx,
        playerFromJson
    ));

    obj->setSettlements(objectListFromJson<core::Settlement>(
        jobj["settlements"].toArray(),
        this->ctx,
        settlementFromJson
    ));

    obj->setUnits(objectListFromJson<core::Unit>(
        jobj["units"].toArray(),
        this->ctx,
        unitFromJson
    ));

    return obj.release();
}

core::DamageType * JsonUnserializer::unserializeDamageType(const QByteArray &data)
{
    QJsonDocument jdoc(parseJson(data));
    return damageTypeFromJson(jdoc.object());
}

core::Faction * JsonUnserializer::unserializeFaction(const QByteArray &data)
{
    QJsonDocument jdoc(parseJson(data));
    return factionsFromJson(jdoc.object(), this->ctx);
}

core::MapNode * JsonUnserializer::unserializeMapNode(const QByteArray &data)
{
    QJsonDocument jdoc(parseJson(data));
    return mapNodeFromJson(jdoc.object(), this->ctx);
}

core::Player * JsonUnserializer::unserializePlayer(const QByteArray &data)
{
    QJsonDocument jdoc(parseJson(data));
    return playerFromJson(jdoc.object(), this->ctx);
}

core::Settlement * JsonUnserializer::unserializeSettlement(const QByteArray &data)
{
    QJsonDocument jdoc(parseJson(data));
    return settlementFromJson(jdoc.object(), this->ctx);
}

core::SettlementType * JsonUnserializer::unserializeSettlementType(const QByteArray &data)
{
    QJsonDocument jdoc(parseJson(data));
    return settlementTypeFromJson(jdoc.object(), this->ctx);
}

core::TerrainType * JsonUnserializer::unserializeTerrainType(const QByteArray &data)
{
    QJsonDocument jdoc(parseJson(data));
    return terrainTypeFromJson(jdoc.object());
}

core::Unit * JsonUnserializer::unserializeUnit(const QByteArray &data)
{
    QJsonDocument jdoc(parseJson(data));
    return unitFromJson(jdoc.object(), this->ctx);
}

core::UnitClass * JsonUnserializer::unserializeUnitClass(const QByteArray &data)
{
    QJsonDocument jdoc(parseJson(data));
    return unitClassFromJson(jdoc.object(), this->ctx);
}

core::UnitLevel * JsonUnserializer::unserializeUnitLevel(const QByteArray &data)
{
    QJsonDocument jdoc(parseJson(data));
    return unitLevelFromJson(jdoc.object());
}

core::UnitType * JsonUnserializer::unserializeUnitType(const QByteArray &data)
{
    QJsonDocument jdoc(parseJson(data));
    return unitTypeFromJson(jdoc.object(), this->ctx);
}

core::Weapon * JsonUnserializer::unserializeWeapon(const QByteArray &data)
{
    QJsonDocument jdoc(parseJson(data));
    return weaponFromJson(jdoc.object(), this->ctx);
}

core::World * JsonUnserializer::unserializeWorld(const QByteArray &data)
{
    QJsonDocument jdoc(parseJson(data));
    QJsonObject jobj = jdoc.object();

    std::unique_ptr<core::World> obj(new core::World());
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setDamageTypes(objectListFromJson<core::DamageType>(
        jobj["damageTypes"].toArray(),
        this->ctx,
        damageTypeFromJson
    ));
    obj->setArmors(objectListFromJson<core::Armor>(
        jobj["armors"].toArray(),
        this->ctx,
        armorFromJson
    ));
    obj->setWeapons(objectListFromJson<core::Weapon>(
        jobj["weapons"].toArray(),
        this->ctx,
        weaponFromJson
    ));
    obj->setTerrainTypes(objectListFromJson<core::TerrainType>(
        jobj["terrainTypes"].toArray(),
        this->ctx,
        terrainTypeFromJson
    ));
    obj->setUnitClasses(objectListFromJson<core::UnitClass>(
        jobj["unitClasses"].toArray(),
        this->ctx,
        unitClassFromJson
    ));
    obj->setUnitLevels(objectListFromJson<core::UnitLevel>(
        jobj["unitLevels"].toArray(),
        this->ctx,
        unitLevelFromJson
    ));
    // UniTypes refer to other UnitTypes so we need to add UnitTypes
    // to the context as soon as they are unserialized
    obj->setUnitTypes(fromQJsonArray<std::vector<core::UnitType *>>(
        jobj["unitTypes"].toArray(),
        [&](const QJsonValue &v)
        {
            core::UnitType *ut = unitTypeFromJson(v.toObject(), this->ctx);
            this->ctx.add(ut);
            return ut;
        }
    ));
    obj->setSettlementTypes(objectListFromJson<core::SettlementType>(
        jobj["settlementTypes"].toArray(),
        this->ctx,
        settlementTypeFromJson
    ));
    obj->setFactions(objectListFromJson<core::Faction>(
        jobj["factions"].toArray(),
        this->ctx,
        factionsFromJson
    ));

    return obj.release();
}

core::WorldSurface * JsonUnserializer::unserializeWorldSurface(const QByteArray &data)
{
    QJsonDocument jdoc(parseJson(data));
    QJsonObject jobj = jdoc.object();

    core::WorldSurface *obj = new core::WorldSurface();
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setTileWidth(jobj["tileWidth"].toInt());
    obj->setTileHeight(jobj["tileHeight"].toInt());

    return obj;
}

QJsonDocument parseJson(const QByteArray &json)
{
    QJsonParseError parseError;

    QJsonDocument doc(QJsonDocument::fromJson(json, &parseError));

    if (parseError.error != QJsonParseError::NoError)
    {
        throw JsonParseError(
            parseError.errorString() + " at " + parseError.offset
        );
    }

    return doc;
}

core::Armor * armorFromJson(const QJsonObject &jobj, Context &ctx)
{
    std::unique_ptr<core::Armor> obj(new core::Armor());
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setDefenses(fromQJsonObject<std::map<const core::DamageType *, int>>(
        jobj["defenses"].toObject(),
        ReferenceResolver<core::DamageType>(ctx),
        qJsonValueToInt
    ));

    return obj.release();
}

core::DamageType * damageTypeFromJson(const QJsonObject &jobj)
{
    std::unique_ptr<core::DamageType> obj(new core::DamageType());
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());

    return obj.release();
}

core::Faction * factionsFromJson(const QJsonObject &jobj, Context &ctx)
{
    ReferenceResolver<core::UnitType> unitTypeRefResolver(ctx);

    std::unique_ptr<core::Faction> obj(new core::Faction());
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setUnitTypes(fromQJsonArray<std::vector<core::UnitType *>>(
        jobj["unitTypes"].toArray(),
        unitTypeRefResolver
    ));
    obj->setRecruits(
        fromQJsonObject<std::map<core::SettlementType *, std::vector<core::UnitType *>>>(
            jobj["recruits"].toObject(),
            ReferenceResolver<core::SettlementType>(ctx),
            [&](const QJsonValue &v)
            {
                return fromQJsonArray<std::vector<core::UnitType *>>(
                    v.toArray(),
                    unitTypeRefResolver
                );
            }
        )
    );

    return obj.release();
}

core::MapNode * mapNodeFromJson(const QJsonObject &jobj, Context &ctx)
{
    std::unique_ptr<core::MapNode> obj(new core::MapNode());
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setTerrainType(resolveReference<core::TerrainType>(
        ctx,
        jobj["terrainType"].toString()
    ));

    return obj.release();
}

std::vector<core::MapNode *> mapNodesFromJson(const QJsonArray &jarr, Context &ctx)
{
    std::vector<core::MapNode *> mapNodes;
    std::vector<std::tuple<core::MapNode *, core::Direction, QString>> neighbours;

    for (QJsonValue jval : jarr)
    {
        QJsonObject jobj = jval.toObject();
        core::MapNode *mn = mapNodeFromJson(jobj, ctx);

        mapNodes.push_back(mn);
        ctx.add(mn);

        // for now just store the references to the neighbours
        // they will be resolved after all mapnodes have been processed
        const QJsonObject jneighbours = jobj["neighbours"].toObject();
        for (auto it = jneighbours.constBegin(); it != jneighbours.constEnd(); it++)
        {
            core::Direction d = core::str2direction(it.key());
            neighbours.push_back(std::make_tuple(mn, d, it.value().toString()));
        }
    }

    core::MapNode *mn;
    core::Direction d;
    QString neighbourName;
    for (const auto& neighbour : neighbours)
    {
        std::tie(mn, d, neighbourName) = neighbour;
        if (neighbourName.isEmpty())
        {
            mn->setNeighbour(d, nullptr);
        }
        else
        {
            mn->setNeighbour(d, resolveReference<core::MapNode>(ctx, neighbourName));
        }
    }

    return mapNodes;
}

core::Player * playerFromJson(const QJsonObject &jobj, Context &ctx)
{
    std::unique_ptr<core::Player> obj(new core::Player());
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setColor(QColor(jobj["color"].toString()));
    obj->setGoldBalance(jobj["goldBalance"].toInt());
    obj->setFaction(resolveReference<core::Faction>(
        ctx,
        jobj["faction"].toString()
    ));

    return obj.release();
}

core::Settlement * settlementFromJson(
    const QJsonObject &jobj,
    Context &ctx
)
{
    std::unique_ptr<core::Settlement> obj(new core::Settlement());
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setType(resolveReference<core::SettlementType>(
        ctx,
        jobj["type"].toString()
    ));
    obj->setMapNode(resolveReference<core::MapNode>(
        ctx,
        jobj["mapNode"].toString()
    ));
    obj->setOwner(resolveReference<core::Player>(
        ctx,
        jobj["owner"].toString()
    ));

    return obj.release();
}

core::SettlementType * settlementTypeFromJson(
    const QJsonObject &jobj,
    Context &ctx
)
{
    std::unique_ptr<core::SettlementType> obj(new core::SettlementType());
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setGoldPerTurn(jobj["goldPerTurn"].toInt());
    obj->setRecruits(fromQJsonArray<std::vector<core::UnitType *>>(
        jobj["recruits"].toArray(),
        ReferenceResolver<core::UnitType>(ctx)
    ));

    return obj.release();
}

core::TerrainType * terrainTypeFromJson(const QJsonObject &jobj)
{
    std::unique_ptr<core::TerrainType> obj(new core::TerrainType());
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());

    return obj.release();
}

core::Unit * unitFromJson(const QJsonObject &jobj, Context &ctx)
{
    std::unique_ptr<core::Unit> obj(new core::Unit());
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setType(resolveReference<core::UnitType>(
        ctx,
        jobj["type"].toString()
    ));
    obj->setMapNode(resolveReference<core::MapNode>(
        ctx,
        jobj["mapNode"].toString()
    ));
    obj->setOwner(resolveReference<core::Player>(
        ctx,
        jobj["owner"].toString()
    ));

    return obj.release();
}

core::UnitClass * unitClassFromJson(const QJsonObject &jobj, Context &ctx)
{
    std::unique_ptr<core::UnitClass> obj(new core::UnitClass());
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setMovementPoints(jobj["movementPoints"].toInt());
    obj->setMovementCosts(fromQJsonObject<std::map<const core::TerrainType *, int>>(
        jobj["movementCosts"].toObject(),
        ReferenceResolver<core::TerrainType>(ctx),
        qJsonValueToInt
    ));
    obj->setAttacks(fromQJsonObject<std::map<const core::TerrainType *, int>>(
        jobj["attacks"].toObject(),
        ReferenceResolver<core::TerrainType>(ctx),
        qJsonValueToInt
    ));
    obj->setDefenses(fromQJsonObject<std::map<const core::TerrainType *, int>>(
        jobj["defenses"].toObject(),
        ReferenceResolver<core::TerrainType>(ctx),
        qJsonValueToInt
    ));

    return obj.release();
}

core::UnitLevel * unitLevelFromJson(const QJsonObject &jobj)
{
    std::unique_ptr<core::UnitLevel> obj(new core::UnitLevel());
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setExperiencePoints(jobj["experiencePoints"].toInt());
    obj->setIndex(jobj["index"].toInt());

    return obj.release();
}

core::UnitType * unitTypeFromJson(const QJsonObject &jobj, Context &ctx)
{
    std::unique_ptr<core::UnitType> obj(new core::UnitType());
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setClass(resolveReference<core::UnitClass>(
        ctx,
        jobj["class"].toString()
    ));
    obj->setLevel(resolveReference<core::UnitLevel>(
        ctx,
        jobj["level"].toString()
    ));
    obj->setHitPoints(jobj["hitPoints"].toInt());
    obj->setRecruitmentCost(jobj["recruitmentCost"].toInt());
    obj->setUpkeepCost(jobj["upkeepCost"].toInt());
    obj->setArmor(resolveReference<core::Armor>(
        ctx,
        jobj["armor"].toString()
    ));
    obj->setWeapons(fromQJsonArray<std::vector<core::Weapon *>>(
        jobj["weapons"].toArray(),
        ReferenceResolver<core::Weapon>(ctx)
    ));
    obj->setUpgrades(fromQJsonArray<std::vector<core::UnitType *>>(
        jobj["upgrades"].toArray(),
        ReferenceResolver<core::UnitType>(ctx)
    ));

    return obj.release();
}

core::Weapon * weaponFromJson(const QJsonObject &jobj, Context &ctx)
{
    std::unique_ptr<core::Weapon> obj(new core::Weapon());
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setRange(jobj["range"].toInt());
    obj->setDamages(fromQJsonObject<std::map<const core::DamageType *, int>>(
        jobj["damages"].toObject(),
        ReferenceResolver<core::DamageType>(ctx),
        qJsonValueToInt
    ));

    return obj.release();
}
