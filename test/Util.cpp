#include "core/Armor.h"
#include "core/DamageType.h"
#include "core/Faction.h"
#include "core/Map.h"
#include "core/MapNode.h"
#include "core/Player.h"
#include "core/Settlement.h"
#include "core/SettlementType.h"
#include "core/TerrainType.h"
#include "core/Unit.h"
#include "core/UnitClass.h"
#include "core/UnitLevel.h"
#include "core/UnitType.h"
#include "core/Weapon.h"
#include "core/World.h"
#include "core/WorldSurface.h"
#include "test/Util.h"

using namespace warmonger;

std::ostream &operator<<(std::ostream &os, const QString &s)
{
    os << s.toStdString();
    return os;
}

template <class GameObject>
void setNames(GameObject obj, QJsonObject &jobj, int i=0)
{
    static const QString objectNameTemplate{"%1_objectName_%2"};
    static const QString displayNameTemplate{"%1_displayName_%2"};

    const QMetaObject *mo = obj->metaObject();

    const QString objectName = objectNameTemplate.arg(mo->className()).arg(i);
    const QString displayName = displayNameTemplate.arg(mo->className()).arg(i);

    obj->setObjectName(objectName);
    jobj["objectName"] = objectName;

    obj->setDisplayName(displayName);
    jobj["displayName"] = displayName;
}

QPair<core::World *, QJsonObject> makeWorld()
{
    core::World *w(new core::World());
    QJsonObject jw;

    setNames(w, jw, 0);

    // DamageType
    core::DamageType *dt0 = new core::DamageType(w);
    QJsonObject jdt0;
    setNames(dt0, jdt0, 0);

    core::DamageType *dt1 = new core::DamageType(w);
    QJsonObject jdt1;
    setNames(dt1, jdt1, 1);

    w->setDamageTypes({dt0, dt1});
    jw["damageTypes"] = QJsonArray({jdt0, jdt1});

    // Armor
    core::Armor *a0 = new core::Armor(w);
    QJsonObject ja0;
    setNames(a0, ja0, 0);

    a0->setDefense(dt0, 22);
    ja0["defenses"] = QJsonObject({
        qMakePair(dt0->objectName(), 22)
    });

    w->setArmors({a0});
    jw["armors"] = QJsonArray({ja0});

    // Weapon
    core::Weapon *w0 = new core::Weapon(w);
    QJsonObject jw0;
    setNames(w0, jw0, 0);

    w0->setRange(0);
    jw0["range"] = 0;

    w0->setDamage(dt0, 10);
    w0->setDamage(dt1, 5);
    jw0["damages"] = QJsonObject({
        qMakePair(dt0->objectName(), 10),
        qMakePair(dt1->objectName(), 5)
    });

    core::Weapon *w1 = new core::Weapon(w);
    QJsonObject jw1;
    setNames(w1, jw1, 1);

    w1->setRange(3);
    jw1["range"] = 3;

    w1->setDamage(dt1, 9);
    jw1["damages"] = QJsonObject({
        qMakePair(dt1->objectName(), 9)
    });

    w->setWeapons({w0, w1});
    jw["weapons"] = QJsonArray({jw0, jw1});

    // TerrainType
    core::TerrainType *tt0 = new core::TerrainType(w);
    QJsonObject jtt0;
    setNames(tt0, jtt0, 0);

    w->setTerrainTypes({tt0});
    jw["terrainTypes"] = QJsonArray({jtt0});

    // UnitClass
    core::UnitClass *uc0 = new core::UnitClass(w);
    QJsonObject juc0;
    setNames(uc0, juc0, 0);

    uc0->setMovementCost(tt0, 8);
    juc0["movementCosts"] = QJsonObject({
        qMakePair(tt0->objectName(), 8)
    });

    uc0->setAttack(tt0, 10);
    juc0["attacks"] = QJsonObject({
        qMakePair(tt0->objectName(), 10)
    });

    uc0->setDefense(tt0, 30);
    juc0["defenses"] = QJsonObject({
        qMakePair(tt0->objectName(), 30)
    });

    w->setUnitClasses({uc0});
    jw["unitClasses"] = QJsonArray({juc0});

    // UnitLevel
    core::UnitLevel *ul0 = new core::UnitLevel(w);
    QJsonObject jul0;
    setNames(ul0, jul0, 0);

    ul0->setExperiencePoints(800);
    jul0["experiencePoints"] = 800;

    ul0->setIndex(1);
    jul0["index"] = 1;

    w->setUnitLevels({ul0});
    jw["unitLevels"] = QJsonArray({jul0});

    // UnitType
    core::UnitType *ut0 = new core::UnitType(w);
    QJsonObject jut0;
    setNames(ut0, jut0, 0);

    ut0->setClass(uc0);
    jut0["class"] = uc0->objectName();

    ut0->setLevel(ul0);
    jut0["level"] = ul0->objectName();

    ut0->setHitPoints(32);
    jut0["hitPoints"] = 32;

    ut0->setRecruitmentCost(200);
    jut0["recruitmentCost"] = 200;

    ut0->setUpkeepCost(20);
    jut0["upkeepCost"] = 20;

    ut0->setArmor(a0);
    jut0["armor"] = a0->objectName();

    ut0->setWeapons({w0, w1});
    jut0["weapons"] = QJsonArray({w0->objectName(), w1->objectName()});

    ut0->setUpgrades({});
    jut0["upgrades"] = QJsonArray({});

    core::UnitType *ut1 = new core::UnitType(w);
    QJsonObject jut1;
    setNames(ut1, jut1, 1);

    ut1->setClass(uc0);
    jut1["class"] = uc0->objectName();

    ut1->setLevel(ul0);
    jut1["level"] = ul0->objectName();

    ut1->setHitPoints(22);
    jut1["hitPoints"] = 22;

    ut1->setRecruitmentCost(100);
    jut1["recruitmentCost"] = 100;

    ut1->setUpkeepCost(10);
    jut1["upkeepCost"] = 10;

    ut1->setArmor(a0);
    jut1["armor"] = a0->objectName();

    ut1->setWeapons({w0});
    jut1["weapons"] = QJsonArray({w0->objectName()});

    ut1->setUpgrades({ut0});
    jut1["upgrades"] = QJsonArray({ut0->objectName()});

    w->setUnitTypes({ut0, ut1});
    jw["unitTypes"] = QJsonArray({jut0, jut1});

    // SettlementType
    core::SettlementType *st0 = new core::SettlementType(w);
    QJsonObject jst0;
    setNames(st0, jst0, 0);

    st0->setGoldPerTurn(7);
    jst0["goldPerTurn"] = 7;

    st0->setRecruits({ut0});
    jst0["recruits"] = QJsonArray({ut0->objectName()});

    w->setSettlementTypes({st0});
    jw["settlementTypes"] = QJsonArray({jst0});

    // Faction
    core::Faction *f0 = new core::Faction(w);
    QJsonObject jf0;
    setNames(f0, jf0, 0);

    f0->setUnitTypes({ut0});
    jf0["unitTypes"] = QJsonArray({ut0->objectName()});

    QMap<core::SettlementType *, QList<core::UnitType *>> recruits;
    recruits[st0] = QList<core::UnitType *>({ut0});
    f0->setRecruits(recruits);
    jf0["recruits"] = QJsonObject({
        qMakePair(st0->objectName(), QJsonArray({ut0->objectName()}))
    });

    w->setFactions({f0});
    jw["factions"] = QJsonArray({jf0});

    return qMakePair(w, jw);
}

QPair<core::Map *, QJsonObject> makeMap()
{
    core::Map *m(new core::Map());
    QJsonObject jm;

    setNames(m, jm, 0);

    const QPair<core::World *, QJsonObject> worlds = makeWorld();
    core::World *w = worlds.first;

    m->setWorld(w);
    jm["world"] = w->objectName();

    m->setMapNodeIndex(2);
    jm["mapNodeIndex"] = 2;

    m->setSettlementIndex(1);
    jm["settlementIndex"] = 1;

    m->setUnitIndex(1);
    jm["unitIndex"] = 1;

    // MapNodes
    core::MapNode *mn0 = new core::MapNode(m);
    QJsonObject jmn0;

    setNames(mn0, jmn0, 0);

    core::TerrainType *tt0 = w->getTerrainTypes()[0];

    mn0->setTerrainType(tt0);
    jmn0["terrainType"] = tt0->objectName();

    core::MapNode *mn1 = new core::MapNode(m);
    QJsonObject jmn1;

    setNames(mn1, jmn1, 1);

    mn1->setTerrainType(tt0);
    jmn1["terrainType"] = tt0->objectName();

    m->setMapNodes({mn0, mn1});
    jm["mapNodes"] = QJsonArray({jmn0, jmn1});

    // MapNode connections
    m->addMapNodeConnection(mn0, mn1, core::Axis::West_East);
    QJsonArray jMapNodeConn;
    jMapNodeConn << mn0->objectName();
    jMapNodeConn << mn1->objectName();
    jMapNodeConn << core::axis2str(core::Axis::West_East);
    jm["mapNodeConnections"] = QJsonArray({jMapNodeConn});

    // Players
    core::Player *p0 = new core::Player(m);
    QJsonObject jp0;

    setNames(p0, jp0, 0);

    QColor color("red");
    p0->setColor(color);
    jp0["color"] = color.name();

    p0->setGoldBalance(142);
    jp0["goldBalance"] = 142;

    core::Faction *f0 = w->getFactions()[0];

    p0->setFaction(f0);
    jp0["faction"] = f0->objectName();

    m->setPlayers({p0});
    jm["players"] = QJsonArray({jp0});

    // Settlements
    core::Settlement *s0 = new core::Settlement(m);
    QJsonObject js0;

    setNames(s0, js0, 0);

    core::SettlementType *st0 = w->getSettlementTypes()[0];

    s0->setType(st0);
    js0["type"] = st0->objectName();

    s0->setMapNode(mn0);
    js0["mapNode"] = mn0->objectName();

    s0->setOwner(p0);
    js0["owner"] = p0->objectName();

    m->setSettlements({s0});
    jm["settlements"] = QJsonArray({js0});

    // Units
    core::Unit *u0 = new core::Unit(m);
    QJsonObject ju0;

    setNames(u0, ju0, 0);

    core::UnitType *ut0 = w->getUnitTypes()[0];

    u0->setType(ut0);
    ju0["type"] = ut0->objectName();

    u0->setMapNode(mn1);
    ju0["mapNode"] = mn1->objectName();

    u0->setOwner(p0);
    ju0["owner"] = p0->objectName();

    u0->setExperiencePoints(100.0);
    ju0["experiencePoints"] = 100.0;

    u0->setHitPoints(30.0);
    ju0["hitPoints"] = 30.0;

    u0->setMovementPoints(16.0);
    ju0["movementPoints"] = 16.0;

    m->setUnits({u0});
    jm["units"] = QJsonArray({ju0});

    return qMakePair(m, jm);
}
