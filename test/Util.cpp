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

std::unique_ptr<core::World> makeWorld()
{
    std::unique_ptr<core::World> w(new core::World());
    w->setObjectName("world1");
    w->setDisplayName("World 1");

    core::DamageType *dt1 = new core::DamageType(w.get());
    dt1->setObjectName("damageType1");
    dt1->setDisplayName("DamageType 1");

    core::DamageType *dt2 = new core::DamageType(w.get());
    dt2->setObjectName("damageType2");
    dt2->setDisplayName("DamageType 2");

    w->setDamageTypes({dt1, dt2});

    core::Armor *a1 = new core::Armor(w.get());
    a1->setObjectName("armor1");
    a1->setDisplayName("Armor 1");
    a1->setDefense(dt1, 22);

    w->setArmors({a1});

    core::Weapon *w1 = new core::Weapon(w.get());
    w1->setObjectName("weapon1");
    w1->setDisplayName("Weapon 1");
    w1->setRange(0);
    w1->setDamage(dt1, 10);
    w1->setDamage(dt2, 5);

    core::Weapon *w2 = new core::Weapon(w.get());
    w2->setObjectName("weapon2");
    w2->setDisplayName("Weapon 2");
    w2->setRange(2);
    w2->setDamage(dt2, 12);

    w->setWeapons({w1, w2});

    core::TerrainType *tt1 = new core::TerrainType(w.get());
    tt1->setObjectName("terrainType1");
    tt1->setDisplayName("TerrainType 1");

    w->setTerrainTypes({tt1});

    core::UnitClass *uc1 = new core::UnitClass(w.get());
    uc1->setObjectName("unitClass1");
    uc1->setDisplayName("UnitClass 1");
    uc1->setMovementCost(tt1, 8);
    uc1->setAttack(tt1, 10);
    uc1->setDefense(tt1, 30);

    w->setUnitClasses({uc1});

    core::UnitLevel *ul1 = new core::UnitLevel(w.get());
    ul1->setObjectName("unitLevel1");
    ul1->setDisplayName("UnitLevel 1");
    ul1->setExperiencePoints(800);
    ul1->setIndex(1);

    w->setUnitLevels({ul1});

    core::UnitType *ut1 = new core::UnitType(w.get());
    ut1->setObjectName("unitType1");
    ut1->setDisplayName("UnitType 1");
    ut1->setClass(uc1);
    ut1->setLevel(ul1);
    ut1->setHitPoints(32);
    ut1->setRecruitmentCost(200);
    ut1->setUpkeepCost(20);
    ut1->setArmor(a1);
    ut1->setWeapons({w1});

    w->setUnitTypes({ut1});

    core::SettlementType *st1 = new core::SettlementType(w.get());
    st1->setObjectName("settlementType1");
    st1->setDisplayName("SettlementType 1");
    st1->setGoldPerTurn(7);
    st1->setRecruits({ut1});

    w->setSettlementTypes({st1});

    core::Faction *f1 = new core::Faction(w.get());
    f1->setObjectName("faction1");
    f1->setDisplayName("Faction 1");
    f1->setUnitTypes({ut1});

    w->setFactions({f1});

    QMap<core::SettlementType *, QList<core::UnitType *>> recruits;
    recruits[st1] = QList<core::UnitType *>({ut1});
    f1->setRecruits(recruits);

    return w;
}
