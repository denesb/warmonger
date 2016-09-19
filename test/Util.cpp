#include "test/Util.h"
#include "core/Army.h"
#include "core/ArmyType.h"
#include "core/Civilization.h"
#include "core/Hexagon.h"
#include "core/MapNode.h"
#include "core/Settlement.h"
#include "core/SettlementType.h"
#include "core/TerrainType.h"
#include "core/Unit.h"
#include "core/UnitType.h"
#include "utils/ToString.h"

using namespace warmonger;

template <class GameObject>
void setNames(GameObject obj, QJsonObject& jobj, int i = 0)
{
    static const QString objectNameTemplate{"%1_objectName_%2"};
    static const QString displayNameTemplate{"%1_displayName_%2"};

    const QMetaObject* mo = obj->metaObject();

    const QString objectName = objectNameTemplate.arg(mo->className()).arg(i);
    const QString displayName = displayNameTemplate.arg(mo->className()).arg(i);

    obj->setObjectName(objectName);
    jobj["objectName"] = objectName;

    obj->setDisplayName(displayName);
    jobj["displayName"] = displayName;
}

std::pair<core::World*, QJsonObject> makeWorld()
{
    core::World* w(new core::World());
    QJsonObject jw;

    setNames(w, jw, 0);

    // TerrainType
    core::TerrainType* tt0 = new core::TerrainType(w);
    QJsonObject jtt0;
    setNames(tt0, jtt0, 0);

    core::TerrainType* tt1 = new core::TerrainType(w);
    QJsonObject jtt1;
    setNames(tt1, jtt1, 1);

    tt1->setHierarchyParent(tt0);
    jtt1["hierarchyParent"] = tt0->objectName();

    w->setTerrainTypes({tt0, tt1});
    jw["terrainTypes"] = QJsonArray({jtt0, jtt1});

    // ArmyType
    core::ArmyType* at0 = new core::ArmyType(w);
    QJsonObject jat0;
    setNames(at0, jat0, 0);

    core::ArmyType* at1 = new core::ArmyType(w);
    QJsonObject jat1;
    setNames(at1, jat1, 1);

    at1->setHierarchyParent(at0);
    jat1["hierarchyParent"] = at0->objectName();

    w->setArmyTypes({at0, at1});
    jw["armyTypes"] = QJsonArray({jat0, jat1});

    // UnitType
    core::UnitType* ut0 = new core::UnitType(w);
    QJsonObject jut0;
    setNames(ut0, jut0, 0);

    ut0->setHitPoints(32);
    jut0["hitPoints"] = 32;

    ut0->setExperiencePoints(200);
    jut0["experiencePoints"] = 200;

    ut0->setMovementPoints(60);
    jut0["movementPoints"] = 60;

    ut0->setRecruitmentCost(200);
    jut0["recruitmentCost"] = 200;

    ut0->setUpkeepCost(20);
    jut0["upkeepCost"] = 20;

    ut0->setUpgrades({});
    jut0["upgrades"] = QJsonArray({});

    ut0->setMovementCost(tt0, 8);
    jut0["movementCosts"] = QJsonObject({qMakePair(tt0->objectName(), 8)});

    core::UnitType* ut1 = new core::UnitType(w);
    QJsonObject jut1;
    setNames(ut1, jut1, 1);

    ut1->setHitPoints(22);
    jut1["hitPoints"] = 22;

    ut1->setExperiencePoints(100);
    jut1["experiencePoints"] = 100;

    ut1->setMovementPoints(40);
    jut1["movementPoints"] = 40;

    ut1->setRecruitmentCost(100);
    jut1["recruitmentCost"] = 100;

    ut1->setUpkeepCost(10);
    jut1["upkeepCost"] = 10;

    ut1->setUpgrades({ut0});
    jut1["upgrades"] = QJsonArray({ut0->objectName()});

    ut1->setMovementCost(tt0, 6);
    jut1["movementCosts"] = QJsonObject({qMakePair(tt0->objectName(), 6)});

    core::UnitType* ut2 = new core::UnitType(w);
    QJsonObject jut2;
    setNames(ut2, jut2, 2);

    ut2->setHierarchyParent(ut1);
    jut2["hierarchyParent"] = ut1->objectName();

    w->setUnitTypes({ut0, ut1, ut2});
    jw["unitTypes"] = QJsonArray({jut0, jut1, jut2});

    // SettlementType
    core::SettlementType* st0 = new core::SettlementType(w);
    QJsonObject jst0;
    setNames(st0, jst0, 0);

    core::SettlementType* st1 = new core::SettlementType(w);
    QJsonObject jst1;
    setNames(st1, jst1, 1);

    st1->setHierarchyParent(st0);
    jst1["hierarchyParent"] = st0->objectName();

    w->setSettlementTypes({st0, st1});
    jw["settlementTypes"] = QJsonArray({jst0, jst1});

    // Civilization
    core::Civilization* c0 = new core::Civilization(w);
    QJsonObject jc0;
    setNames(c0, jc0, 0);

    c0->setUnitTypes({ut0});
    jc0["unitTypes"] = QJsonArray({ut0->objectName()});

    std::map<core::SettlementType*, std::vector<core::UnitType*>> recruits;
    recruits[st0] = std::vector<core::UnitType*>({ut0});

    w->setCivilizations({c0});
    jw["civilizations"] = QJsonArray({jc0});

    return std::make_pair(w, jw);
}

std::pair<core::CampaignMap*, QJsonObject> makeMap()
{
    core::CampaignMap* m(new core::CampaignMap());
    QJsonObject jm;

    setNames(m, jm, 0);

    const std::pair<core::World*, QJsonObject> worlds = makeWorld();
    core::World* w = worlds.first;

    m->setWorld(w);
    jm["world"] = w->objectName();

    m->setMapNodeIndex(2);
    jm["mapNodeIndex"] = 2;

    m->setSettlementIndex(1);
    jm["settlementIndex"] = 1;

    m->setUnitIndex(1);
    jm["unitIndex"] = 1;

    m->setArmyIndex(1);
    jm["armyIndex"] = 1;

    // MapNodes
    core::MapNode* mn0 = new core::MapNode(m);
    QJsonObject jmn0;

    setNames(mn0, jmn0, 0);

    core::TerrainType* tt0 = w->getTerrainTypes()[0];

    mn0->setTerrainType(tt0);
    jmn0["terrainType"] = tt0->objectName();

    core::MapNode* mn1 = new core::MapNode(m);
    QJsonObject jmn1;

    setNames(mn1, jmn1, 1);

    mn1->setTerrainType(tt0);
    jmn1["terrainType"] = tt0->objectName();

    // MapNode neighbours
    mn0->setNeighbour(core::Direction::West, mn1);
    mn1->setNeighbour(core::Direction::East, mn0);
    jmn0["neighbours"] = QJsonObject{{"West", mn1->objectName()},
        {"NorthWest", ""},
        {"NorthEast", ""},
        {"SouthEast", ""},
        {"SouthWest", ""},
        {"East", ""}};
    jmn1["neighbours"] = QJsonObject{{"West", ""},
        {"NorthWest", ""},
        {"NorthEast", ""},
        {"SouthEast", ""},
        {"SouthWest", ""},
        {"East", mn0->objectName()}};

    m->setMapNodes({mn0, mn1});
    jm["mapNodes"] = QJsonArray({jmn0, jmn1});

    // Factions
    core::Faction* f0 = new core::Faction(m);
    QJsonObject jf0;

    setNames(f0, jf0, 0);

    QColor color("red");
    f0->setColor(color);
    jf0["color"] = color.name();

    f0->setGoldBalance(142);
    jf0["goldBalance"] = 142;

    core::Civilization* c0 = w->getCivilizations()[0];

    f0->setCivilization(c0);
    jf0["civilization"] = c0->objectName();

    m->setFactions({f0});
    jm["factions"] = QJsonArray({jf0});

    // Settlements
    core::Settlement* s0 = new core::Settlement(m);
    QJsonObject js0;

    setNames(s0, js0, 0);

    core::SettlementType* st0 = w->getSettlementTypes()[0];

    s0->setType(st0);
    js0["type"] = st0->objectName();

    s0->setMapNode(mn0);
    js0["mapNode"] = mn0->objectName();

    s0->setOwner(f0);
    js0["owner"] = f0->objectName();

    m->setSettlements({s0});
    jm["settlements"] = QJsonArray({js0});

    // Units
    core::Unit* u0 = new core::Unit(m);
    QJsonObject ju0;

    setNames(u0, ju0, 0);

    core::UnitType* ut0 = w->getUnitTypes()[0];

    u0->setType(ut0);
    ju0["type"] = ut0->objectName();

    u0->setMapNode(mn1);
    ju0["mapNode"] = mn1->objectName();

    u0->setOwner(f0);
    ju0["owner"] = f0->objectName();

    u0->setExperiencePoints(100.0);
    ju0["experiencePoints"] = 100.0;

    u0->setHitPoints(30.0);
    ju0["hitPoints"] = 30.0;

    u0->setMovementPoints(16.0);
    ju0["movementPoints"] = 16.0;

    m->setUnits({u0});
    jm["units"] = QJsonArray({ju0});

    // Armies
    core::Army* a0 = new core::Army(m);
    QJsonObject ja0;

    setNames(a0, ja0, 0);

    core::ArmyType* at0 = w->getArmyTypes()[0];

    a0->setType(at0);
    ja0["type"] = at0->objectName();

    a0->setMapNode(mn1);
    ja0["mapNode"] = mn1->objectName();

    a0->setOwner(f0);
    ja0["owner"] = f0->objectName();

    a0->setUnits({u0});
    ja0["units"] = QJsonArray({u0->objectName()});

    m->setArmies({a0});
    jm["armies"] = QJsonArray({ja0});

    return std::make_pair(m, jm);
}
