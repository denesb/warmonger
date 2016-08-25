#include <algorithm>
#include <functional>
#include <memory>
#include <vector>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "core/CampaignMap.h"
#include "core/World.h"
#include "io/JsonUnserializer.h"
#include "utils/Exception.h"
#include "utils/Hexagon.h"

namespace warmonger {
namespace io {

QJsonDocument parseJson(const QByteArray& json);

core::Army* armyFromJson(const QJsonObject& jobj, Context& ctx);
core::ArmyType* armyTypeFromJson(const QJsonObject& jobj, Context& ctx);
core::Civilization* civilizationsFromJson(const QJsonObject& jobj, Context& ctx);
core::Faction* factionFromJson(const QJsonObject& jobj, Context& ctx);
core::MapNode* mapNodeFromJson(const QJsonObject& jobj, Context& ctx);
std::vector<core::MapNode*> mapNodesFromJson(const QJsonArray& jarr, Context& ctx);
core::Settlement* settlementFromJson(const QJsonObject& jobj, Context& ctx);
core::SettlementType* settlementTypeFromJson(const QJsonObject& jobj, Context& ctx);
core::TerrainType* terrainTypeFromJson(const QJsonObject& jobj);
core::Unit* unitFromJson(const QJsonObject& jobj, Context& ctx);
core::UnitType* unitTypeFromJson(const QJsonObject& jobj, Context& ctx);


/**
 * Resolve reference `name` to `Type`.
 */
template <typename Type>
Type * resolveReference(Context& ctx, const QString& name)
{
    Type *obj = ctx.get<Type *>(name);
    if (obj == nullptr)
    {
        QString className(Type::staticMetaObject.className());
        throw utils::ValueError(
            QString("Unable to resolve reference `%1' to `%2'")
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
    ReferenceResolver(Context& ctx) :
        ctx(ctx)
    {
    }

    Type* operator()(const QString& n) const
    {
        return resolveReference<Type>(this->ctx, n);
    }

    Type* operator()(const QJsonValue& v) const
    {
        return resolveReference<Type>(this->ctx, v.toString());
    }

private:
    Context& ctx;
};

/**
 * Convert QJsonArray to Container.
 *
 * Container must be an STL-compatible container having a push_back
 * method. The convertFunc function converts QJsonValue to
 * Container::value_type.
 */
template<typename Container, typename ConvertFunc>
Container fromQJsonArray(const QJsonArray& array, ConvertFunc convertFunc)
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
    const QJsonObject& object,
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
inline int qJsonValueToInt(const QJsonValue& value)
{
    return value.toInt();
}

template <class T>
std::vector<T*> objectListFromJson(
    const QJsonArray& array,
    Context& ctx,
    std::function<T* (const QJsonObject& )> fromJsonFunc
)
{
    std::vector<T*> objects = fromQJsonArray<std::vector<T*>>(
        array,
        [&](const QJsonValue& v)
        {
            return fromJsonFunc(v.toObject());
        }
    );

    std::for_each(
        objects.begin(),
        objects.end(),
        std::bind(&Context::add,& ctx, std::placeholders::_1)
    );

    return objects;
}

template <class T>
std::vector<T*> objectListFromJson(
    const QJsonArray& array,
    Context& ctx,
    std::function<T* (const QJsonObject& , Context& ctx)> fromJsonFunc
)
{
    std::function<T* (const QJsonObject& )> func = std::bind(
        fromJsonFunc,
        std::placeholders::_1,
        ctx
    );
    return objectListFromJson<T>(array, ctx, func);
}

core::Army* JsonUnserializer::unserializeArmy(const QByteArray& data)
{
    QJsonDocument jdoc(parseJson(data));
    return armyFromJson(jdoc.object(), this->ctx);
}

core::ArmyType* JsonUnserializer::unserializeArmyType(const QByteArray& data)
{
    QJsonDocument jdoc(parseJson(data));
    return armyTypeFromJson(jdoc.object(), this->ctx);
}

core::CampaignMap* JsonUnserializer::unserializeCampaignMap(const QByteArray& data)
{
    QJsonDocument jdoc(parseJson(data));
    QJsonObject jobj = jdoc.object();

    std::unique_ptr<core::CampaignMap> obj(new core::CampaignMap());

    obj->setObjectName(jobj["objectName"].toString());

    obj->setDisplayName(jobj["displayName"].toString());

    obj->setWorld(resolveReference<core::World>(this->ctx, jobj["world"].toString()));

    obj->setMapNodeIndex(jobj["mapNodeIndex"].toInt());

    obj->setSettlementIndex(jobj["settlementIndex"].toInt());

    obj->setUnitIndex(jobj["unitIndex"].toInt());

    obj->setMapNodes(mapNodesFromJson(jobj["mapNodes"].toArray(), this->ctx));

    obj->setFactions(objectListFromJson<core::Faction>(
        jobj["factions"].toArray(),
        this->ctx,
        factionFromJson
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

    obj->setArmies(objectListFromJson<core::Army>(
        jobj["armies"].toArray(),
        this->ctx,
        armyFromJson
    ));

    return obj.release();
}

core::Civilization * JsonUnserializer::unserializeCivilization(const QByteArray& data)
{
    QJsonDocument jdoc(parseJson(data));
    return civilizationsFromJson(jdoc.object(), this->ctx);
}

core::MapNode * JsonUnserializer::unserializeMapNode(const QByteArray& data)
{
    QJsonDocument jdoc(parseJson(data));
    return mapNodeFromJson(jdoc.object(), this->ctx);
}

core::Faction * JsonUnserializer::unserializeFaction(const QByteArray& data)
{
    QJsonDocument jdoc(parseJson(data));
    return factionFromJson(jdoc.object(), this->ctx);
}

core::Settlement * JsonUnserializer::unserializeSettlement(const QByteArray& data)
{
    QJsonDocument jdoc(parseJson(data));
    return settlementFromJson(jdoc.object(), this->ctx);
}

core::SettlementType * JsonUnserializer::unserializeSettlementType(const QByteArray& data)
{
    QJsonDocument jdoc(parseJson(data));
    return settlementTypeFromJson(jdoc.object(), this->ctx);
}

core::TerrainType * JsonUnserializer::unserializeTerrainType(const QByteArray& data)
{
    QJsonDocument jdoc(parseJson(data));
    return terrainTypeFromJson(jdoc.object());
}

core::Unit * JsonUnserializer::unserializeUnit(const QByteArray& data)
{
    QJsonDocument jdoc(parseJson(data));
    return unitFromJson(jdoc.object(), this->ctx);
}

core::UnitType * JsonUnserializer::unserializeUnitType(const QByteArray& data)
{
    QJsonDocument jdoc(parseJson(data));
    return unitTypeFromJson(jdoc.object(), this->ctx);
}

core::World * JsonUnserializer::unserializeWorld(const QByteArray& data)
{
    QJsonDocument jdoc(parseJson(data));
    QJsonObject jobj = jdoc.object();

    std::unique_ptr<core::World> obj(new core::World());
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setArmyTypes(objectListFromJson<core::ArmyType>(
        jobj["armyTypes"].toArray(),
        this->ctx,
        armyTypeFromJson
    ));
    obj->setTerrainTypes(objectListFromJson<core::TerrainType>(
        jobj["terrainTypes"].toArray(),
        this->ctx,
        terrainTypeFromJson
    ));
    // UniTypes refer to other UnitTypes so we need to add UnitTypes
    // to the context as soon as they are unserialized
    obj->setUnitTypes(fromQJsonArray<std::vector<core::UnitType *>>(
        jobj["unitTypes"].toArray(),
        [&](const QJsonValue& v)
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
    obj->setCivilizations(objectListFromJson<core::Civilization>(
        jobj["civilizations"].toArray(),
        this->ctx,
        civilizationsFromJson
    ));

    return obj.release();
}

QJsonDocument parseJson(const QByteArray& json)
{
    QJsonParseError parseError;

    QJsonDocument doc(QJsonDocument::fromJson(json,& parseError));

    if (parseError.error != QJsonParseError::NoError)
    {
        throw utils::ValueError(parseError.errorString() + " at " + parseError.offset);
    }

    return doc;
}

core::ArmyType* armyTypeFromJson(const QJsonObject& jobj, Context&)
{
    std::unique_ptr<core::ArmyType> obj(new core::ArmyType());
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());

    return obj.release();
}
core::Army* armyFromJson(const QJsonObject& jobj, Context& ctx)
{
    std::unique_ptr<core::Army> obj(new core::Army());
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setMapNode(resolveReference<core::MapNode>(
        ctx,
        jobj["mapNode"].toString()
    ));
    obj->setOwner(resolveReference<core::Faction>(
        ctx,
        jobj["owner"].toString()
    ));
    obj->setUnits(fromQJsonArray<std::vector<core::Unit *>>(
        jobj["units"].toArray(),
        ReferenceResolver<core::Unit>(ctx)
    ));

    return obj.release();
}

core::Civilization* civilizationsFromJson(const QJsonObject& jobj, Context& ctx)
{
    ReferenceResolver<core::UnitType> unitTypeRefResolver(ctx);

    std::unique_ptr<core::Civilization> obj(new core::Civilization());
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
            [&](const QJsonValue& v)
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

core::Faction* factionFromJson(const QJsonObject& jobj, Context& ctx)
{
    std::unique_ptr<core::Faction> obj(new core::Faction());
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setColor(QColor(jobj["color"].toString()));
    obj->setGoldBalance(jobj["goldBalance"].toInt());
    obj->setCivilization(resolveReference<core::Civilization>(
        ctx,
        jobj["civilization"].toString()
    ));

    return obj.release();
}

core::MapNode* mapNodeFromJson(const QJsonObject& jobj, Context& ctx)
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

std::vector<core::MapNode*> mapNodesFromJson(const QJsonArray& jarr, Context& ctx)
{
    std::vector<core::MapNode*> mapNodes;
    std::vector<std::tuple<core::MapNode*, utils::Direction, QString>> neighbours;

    for (QJsonValue jval : jarr)
    {
        QJsonObject jobj = jval.toObject();
        core::MapNode* mn = mapNodeFromJson(jobj, ctx);

        mapNodes.push_back(mn);
        ctx.add(mn);

        // for now just store the references to the neighbours
        // they will be resolved after all mapnodes have been processed
        const QJsonObject jneighbours = jobj["neighbours"].toObject();
        for (auto it = jneighbours.constBegin(); it != jneighbours.constEnd(); it++)
        {
            utils::Direction d = utils::str2direction(it.key());
            neighbours.push_back(std::make_tuple(mn, d, it.value().toString()));
        }
    }

    core::MapNode* mn;
    utils::Direction d;
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

core::Settlement* settlementFromJson(
    const QJsonObject& jobj,
    Context& ctx
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
    obj->setOwner(resolveReference<core::Faction>(
        ctx,
        jobj["owner"].toString()
    ));

    return obj.release();
}

core::SettlementType* settlementTypeFromJson(
    const QJsonObject& jobj,
    Context& ctx
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

core::TerrainType* terrainTypeFromJson(const QJsonObject& jobj)
{
    std::unique_ptr<core::TerrainType> obj(new core::TerrainType());
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());

    return obj.release();
}

core::Unit* unitFromJson(const QJsonObject& jobj, Context& ctx)
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
    obj->setOwner(resolveReference<core::Faction>(
        ctx,
        jobj["owner"].toString()
    ));

    return obj.release();
}

core::UnitType* unitTypeFromJson(const QJsonObject& jobj, Context& ctx)
{
    std::unique_ptr<core::UnitType> obj(new core::UnitType());
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setHitPoints(jobj["hitPoints"].toInt());
    obj->setExperiencePoints(jobj["experiencePoints"].toInt());
    obj->setMovementPoints(jobj["movementPoints"].toInt());
    obj->setRecruitmentCost(jobj["recruitmentCost"].toInt());
    obj->setUpkeepCost(jobj["upkeepCost"].toInt());
    obj->setMovementCosts(fromQJsonObject<std::map<core::TerrainType*, int>>(
        jobj["movementCosts"].toObject(),
        ReferenceResolver<core::TerrainType>(ctx),
        qJsonValueToInt
    ));
    obj->setUpgrades(fromQJsonArray<std::vector<core::UnitType *>>(
        jobj["upgrades"].toArray(),
        ReferenceResolver<core::UnitType>(ctx)
    ));

    return obj.release();
}

} // namespace io
} // namespace warmonger
