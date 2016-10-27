#include <algorithm>
#include <functional>
#include <memory>
#include <vector>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "core/CampaignMap.h"
#include "core/Hexagon.h"
#include "core/World.h"
#include "io/JsonUnserializer.h"
#include "utils/Exception.h"

namespace warmonger {
namespace io {

QJsonDocument parseJson(const QByteArray& json);

core::Army* armyFromJson(const QJsonObject& jobj, Context& ctx);
core::ArmyType* armyTypeFromJson(const QJsonObject& jobj, Context& ctx);
core::Banner* bannerFromJson(const QJsonObject& jobj, Context& ctx);
core::Civilization* civilizationsFromJson(const QJsonObject& jobj, Context& ctx);
core::Faction* factionFromJson(const QJsonObject& jobj, Context& ctx);
core::MapNode* mapNodeFromJson(const QJsonObject& jobj, Context& ctx);
std::vector<core::MapNode*> mapNodesFromJson(const QJsonArray& jarr, Context& ctx);
core::Settlement* settlementFromJson(const QJsonObject& jobj, Context& ctx);
core::SettlementType* settlementTypeFromJson(const QJsonObject& jobj, Context& ctx);
core::TerrainType* terrainTypeFromJson(const QJsonObject& jobj, Context& ctx);
core::Unit* unitFromJson(const QJsonObject& jobj, Context& ctx);
core::UnitType* unitTypeFromJson(const QJsonObject& jobj, Context& ctx);

/**
 * Resolve reference `name` to `Type`.
 */
template <typename Type>
Type* resolveReference(Context& ctx, const QString& name)
{
    Type* obj = ctx.get<Type*>(name);
    if (obj == nullptr)
    {
        QString className(Type::staticMetaObject.className());
        throw utils::ValueError(QString("Unable to resolve reference `%1' to `%2'").arg(name).arg(className));
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
    ReferenceResolver(Context& ctx)
        : ctx(ctx)
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
template <typename Container, typename ConvertFunc>
Container fromQJsonArray(const QJsonArray& array, ConvertFunc convertFunc)
{
    Container container;

    std::transform(array.begin(), array.end(), std::back_inserter(container), convertFunc);

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
template <typename Container, typename ConvertKeyFunc, typename ConvertValueFunc>
Container fromQJsonObject(const QJsonObject& object, ConvertKeyFunc convertKey, ConvertValueFunc convertValue)
{
    Container container;

    for (auto it = object.begin(); it != object.end(); it++)
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
    const QJsonArray& array, Context& ctx, std::function<T*(const QJsonObject&)> fromJsonFunc)
{
    std::vector<T*> objects =
        fromQJsonArray<std::vector<T*>>(array, [&](const QJsonValue& v) { return fromJsonFunc(v.toObject()); });

    std::for_each(objects.begin(), objects.end(), std::bind(&Context::add, &ctx, std::placeholders::_1));

    return objects;
}

template <class T>
std::vector<T*> objectListFromJson(
    const QJsonArray& array, Context& ctx, std::function<T*(const QJsonObject&, Context& ctx)> fromJsonFunc)
{
    std::function<T*(const QJsonObject&)> func = std::bind(fromJsonFunc, std::placeholders::_1, ctx);
    return objectListFromJson<T>(array, ctx, func);
}

template <class T>
std::vector<T*> hierarchyNodesFromJson(
    const QJsonArray& json, std::function<T*(const QJsonObject&, Context&)> fromJson, Context& ctx)
{
    return fromQJsonArray<std::vector<T*>>(json, [&](const QJsonValue& v) {
        T* obj = fromJson(v.toObject(), ctx);
        ctx.add(obj);
        return obj;
    });
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

core::Banner* JsonUnserializer::unserializeBanner(const QByteArray& data)
{
    QJsonDocument jdoc(parseJson(data));
    return bannerFromJson(jdoc.object(), this->ctx);
}

core::CampaignMap* JsonUnserializer::unserializeCampaignMap(const QByteArray& data)
{
    QJsonDocument jdoc(parseJson(data));
    QJsonObject jobj = jdoc.object();

    std::unique_ptr<core::CampaignMap> obj(new core::CampaignMap());

    obj->setObjectName(jobj["objectName"].toString());

    obj->setDisplayName(jobj["displayName"].toString());

    obj->setWorld(resolveReference<core::World>(this->ctx, jobj["world"].toString()));

    obj->setMapNodes(mapNodesFromJson(jobj["mapNodes"].toArray(), this->ctx));

    obj->setFactions(objectListFromJson<core::Faction>(jobj["factions"].toArray(), this->ctx, factionFromJson));

    obj->setSettlements(
        objectListFromJson<core::Settlement>(jobj["settlements"].toArray(), this->ctx, settlementFromJson));

    obj->setUnits(objectListFromJson<core::Unit>(jobj["units"].toArray(), this->ctx, unitFromJson));

    obj->setArmies(objectListFromJson<core::Army>(jobj["armies"].toArray(), this->ctx, armyFromJson));

    return obj.release();
}

core::Civilization* JsonUnserializer::unserializeCivilization(const QByteArray& data)
{
    QJsonDocument jdoc(parseJson(data));
    return civilizationsFromJson(jdoc.object(), this->ctx);
}

core::MapNode* JsonUnserializer::unserializeMapNode(const QByteArray& data)
{
    QJsonDocument jdoc(parseJson(data));
    return mapNodeFromJson(jdoc.object(), this->ctx);
}

core::Faction* JsonUnserializer::unserializeFaction(const QByteArray& data)
{
    QJsonDocument jdoc(parseJson(data));
    return factionFromJson(jdoc.object(), this->ctx);
}

core::Settlement* JsonUnserializer::unserializeSettlement(const QByteArray& data)
{
    QJsonDocument jdoc(parseJson(data));
    return settlementFromJson(jdoc.object(), this->ctx);
}

core::SettlementType* JsonUnserializer::unserializeSettlementType(const QByteArray& data)
{
    QJsonDocument jdoc(parseJson(data));
    return settlementTypeFromJson(jdoc.object(), this->ctx);
}

core::TerrainType* JsonUnserializer::unserializeTerrainType(const QByteArray& data)
{
    QJsonDocument jdoc(parseJson(data));
    return terrainTypeFromJson(jdoc.object(), this->ctx);
}

core::Unit* JsonUnserializer::unserializeUnit(const QByteArray& data)
{
    QJsonDocument jdoc(parseJson(data));
    return unitFromJson(jdoc.object(), this->ctx);
}

core::UnitType* JsonUnserializer::unserializeUnitType(const QByteArray& data)
{
    QJsonDocument jdoc(parseJson(data));
    return unitTypeFromJson(jdoc.object(), this->ctx);
}

core::World* JsonUnserializer::unserializeWorld(const QByteArray& data)
{
    QJsonDocument jdoc(parseJson(data));
    QJsonObject jobj = jdoc.object();

    std::unique_ptr<core::World> obj(new core::World());

    obj->setObjectName(jobj["objectName"].toString());

    obj->setDisplayName(jobj["displayName"].toString());

    obj->setArmyTypes(hierarchyNodesFromJson<core::ArmyType>(jobj["armyTypes"].toArray(), armyTypeFromJson, this->ctx));

    obj->setTerrainTypes(
        hierarchyNodesFromJson<core::TerrainType>(jobj["terrainTypes"].toArray(), terrainTypeFromJson, this->ctx));

    obj->setUnitTypes(hierarchyNodesFromJson<core::UnitType>(jobj["unitTypes"].toArray(), unitTypeFromJson, this->ctx));

    obj->setSettlementTypes(hierarchyNodesFromJson<core::SettlementType>(
        jobj["settlementTypes"].toArray(), settlementTypeFromJson, this->ctx));

    obj->setCivilizations(
        objectListFromJson<core::Civilization>(jobj["civilizations"].toArray(), this->ctx, civilizationsFromJson));

    obj->setBanners(hierarchyNodesFromJson<core::Banner>(jobj["banners"].toArray(), bannerFromJson, this->ctx));

    return obj.release();
}

QJsonDocument parseJson(const QByteArray& json)
{
    QJsonParseError parseError;

    QJsonDocument doc(QJsonDocument::fromJson(json, &parseError));

    if (parseError.error != QJsonParseError::NoError)
    {
        throw utils::ValueError(parseError.errorString() + " at " + parseError.offset);
    }

    return doc;
}

core::ArmyType* armyTypeFromJson(const QJsonObject& jobj, Context& ctx)
{
    std::unique_ptr<core::ArmyType> obj(new core::ArmyType());

    if (jobj.contains("hierarchyParent"))
    {
        obj->setHierarchyParent(resolveReference<core::ArmyType>(ctx, jobj["hierarchyParent"].toString()));
    }

    obj->setObjectName(jobj["objectName"].toString());

    if (jobj.contains("displayName"))
        obj->setDisplayName(jobj["displayName"].toString());

    return obj.release();
}

core::Banner* bannerFromJson(const QJsonObject& jobj, Context& ctx)
{
    std::unique_ptr<core::Banner> obj(new core::Banner());

    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    if (jobj.contains("civilizations"))
        obj->setCivilizations(fromQJsonArray<std::vector<core::Civilization*>>(
            jobj["civilizations"].toArray(), ReferenceResolver<core::Civilization>(ctx)));

    return obj.release();
}

core::Army* armyFromJson(const QJsonObject& jobj, Context& ctx)
{
    std::unique_ptr<core::Army> obj(new core::Army());
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setType(resolveReference<core::ArmyType>(ctx, jobj["type"].toString()));
    obj->setMapNode(resolveReference<core::MapNode>(ctx, jobj["mapNode"].toString()));
    obj->setOwner(resolveReference<core::Faction>(ctx, jobj["owner"].toString()));
    obj->setUnits(
        fromQJsonArray<std::vector<core::Unit*>>(jobj["units"].toArray(), ReferenceResolver<core::Unit>(ctx)));

    return obj.release();
}

core::Civilization* civilizationsFromJson(const QJsonObject& jobj, Context& ctx)
{
    ReferenceResolver<core::UnitType> unitTypeRefResolver(ctx);

    std::unique_ptr<core::Civilization> obj(new core::Civilization());
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setUnitTypes(fromQJsonArray<std::vector<core::UnitType*>>(jobj["unitTypes"].toArray(), unitTypeRefResolver));

    return obj.release();
}

core::Faction* factionFromJson(const QJsonObject& jobj, Context& ctx)
{
    std::unique_ptr<core::Faction> obj(new core::Faction());
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setPrimaryColor(QColor(jobj["primaryColor"].toString()));
    obj->setSecondaryColor(QColor(jobj["secondaryColor"].toString()));
    obj->setBanner(resolveReference<core::Banner>(ctx, jobj["banner"].toString()));
    obj->setCivilization(resolveReference<core::Civilization>(ctx, jobj["civilization"].toString()));

    return obj.release();
}

core::MapNode* mapNodeFromJson(const QJsonObject& jobj, Context& ctx)
{
    std::unique_ptr<core::MapNode> obj(new core::MapNode());
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setTerrainType(resolveReference<core::TerrainType>(ctx, jobj["terrainType"].toString()));

    return obj.release();
}

std::vector<core::MapNode*> mapNodesFromJson(const QJsonArray& jarr, Context& ctx)
{
    std::vector<core::MapNode*> mapNodes;
    std::vector<std::tuple<core::MapNode*, core::Direction, QString>> neighbours;

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
            core::Direction d = core::str2direction(it.key());
            neighbours.push_back(std::make_tuple(mn, d, it.value().toString()));
        }
    }

    core::MapNode* mn;
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

core::Settlement* settlementFromJson(const QJsonObject& jobj, Context& ctx)
{
    std::unique_ptr<core::Settlement> obj(new core::Settlement());
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setType(resolveReference<core::SettlementType>(ctx, jobj["type"].toString()));
    obj->setMapNode(resolveReference<core::MapNode>(ctx, jobj["mapNode"].toString()));
    obj->setOwner(resolveReference<core::Faction>(ctx, jobj["owner"].toString()));

    return obj.release();
}

core::SettlementType* settlementTypeFromJson(const QJsonObject& jobj, Context& ctx)
{
    std::unique_ptr<core::SettlementType> obj(new core::SettlementType());

    if (jobj.contains("hierarchyParent"))
    {
        obj->setHierarchyParent(resolveReference<core::SettlementType>(ctx, jobj["hierarchyParent"].toString()));
    }

    obj->setObjectName(jobj["objectName"].toString());

    if (jobj.contains("displayName"))
        obj->setDisplayName(jobj["displayName"].toString());

    return obj.release();
}

core::TerrainType* terrainTypeFromJson(const QJsonObject& jobj, Context& ctx)
{
    std::unique_ptr<core::TerrainType> obj(new core::TerrainType());

    if (jobj.contains("hierarchyParent"))
    {
        obj->setHierarchyParent(resolveReference<core::TerrainType>(ctx, jobj["hierarchyParent"].toString()));
    }

    obj->setObjectName(jobj["objectName"].toString());

    if (jobj.contains("displayName"))
        obj->setDisplayName(jobj["displayName"].toString());

    return obj.release();
}

core::Unit* unitFromJson(const QJsonObject& jobj, Context& ctx)
{
    std::unique_ptr<core::Unit> obj(new core::Unit());
    obj->setObjectName(jobj["objectName"].toString());
    obj->setDisplayName(jobj["displayName"].toString());
    obj->setType(resolveReference<core::UnitType>(ctx, jobj["type"].toString()));
    obj->setMapNode(resolveReference<core::MapNode>(ctx, jobj["mapNode"].toString()));
    obj->setOwner(resolveReference<core::Faction>(ctx, jobj["owner"].toString()));

    return obj.release();
}

core::UnitType* unitTypeFromJson(const QJsonObject& jobj, Context& ctx)
{
    std::unique_ptr<core::UnitType> obj(new core::UnitType());

    obj->setObjectName(jobj["objectName"].toString());

    if (jobj.contains("hierarchyParent"))
    {
        obj->setHierarchyParent(resolveReference<core::UnitType>(ctx, jobj["hierarchyParent"].toString()));
    }

    if (jobj.contains("displayName"))
        obj->setDisplayName(jobj["displayName"].toString());

    if (jobj.contains("hitPoints"))
        obj->setHitPoints(jobj["hitPoints"].toInt());

    if (jobj.contains("experiencePoints"))
        obj->setExperiencePoints(jobj["experiencePoints"].toInt());

    if (jobj.contains("movementPoints"))
        obj->setMovementPoints(jobj["movementPoints"].toInt());

    if (jobj.contains("recruitmentCost"))
        obj->setRecruitmentCost(jobj["recruitmentCost"].toInt());

    if (jobj.contains("upkeepCost"))
        obj->setUpkeepCost(jobj["upkeepCost"].toInt());

    if (jobj.contains("movementCosts"))
        obj->setMovementCosts(fromQJsonObject<std::map<core::TerrainType*, int>>(
            jobj["movementCosts"].toObject(), ReferenceResolver<core::TerrainType>(ctx), qJsonValueToInt));

    if (jobj.contains("upgrades"))
        obj->setUpgrades(fromQJsonArray<std::vector<core::UnitType*>>(
            jobj["upgrades"].toArray(), ReferenceResolver<core::UnitType>(ctx)));

    return obj.release();
}

} // namespace io
} // namespace warmonger
