#include "core/QVariantUtil.h"
#include "core/UnitType.h"

using namespace warmonger::core;

static const QString loggerName{"core.UnitType"};

UnitType::UnitType(QObject *parent) :
    QObject(parent),
    klass(nullptr),
    level(nullptr),
    hitPoints(0),
    recruitmentCost(0),
    upkeepCost(0),
    armor(nullptr),
    weapons(),
    attackSkills(),
    defenseSkills(),
    upgrades()
{
}

QString UnitType::getDisplayName() const
{
    return this->displayName;
}

void UnitType::setDisplayName(const QString &displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
}

UnitClass *UnitType::getClass() const
{
    return this->klass;
}

void UnitType::setClass(UnitClass *klass)
{
    if (this->klass != klass)
    {
        this->klass = klass;
        emit classChanged();
    }
}

UnitLevel * UnitType::getLevel() const
{
    return this->level;
}

void UnitType::setLevel(UnitLevel *level)
{
    if (this->level != level)
    {
        this->level = level;
        emit levelChanged();
    }
}

int UnitType::getHitPoints() const
{
    return this->hitPoints;
}

void UnitType::setHitPoints(int hitPoints)
{
    if (this->hitPoints != hitPoints)
    {
        this->hitPoints = hitPoints;
        emit hitPointsChanged();
    }
}

int UnitType::getRecruitmentCost() const
{
    return this->recruitmentCost;
}

void UnitType::setRecruitmentCost(int recruitmentCost)
{
    if (this->recruitmentCost != recruitmentCost)
    {
        this->recruitmentCost = recruitmentCost;
        emit recruitmentCostChanged();
    }
}

int UnitType::getUpkeepCost() const
{
    return this->upkeepCost;
}

void UnitType::setUpkeepCost(int upkeepCost)
{
    if (this->upkeepCost != upkeepCost)
    {
        this->upkeepCost = upkeepCost;
        emit upkeepCostChanged();
    }
}

Armor * UnitType::getArmor() const
{
    return this->armor;
}

void UnitType::setArmor(Armor *armor)
{
    if (this->armor != armor)
    {
        this->armor = armor;
        emit armorChanged();
    }
}

std::vector<Weapon *> UnitType::getWeapons() const
{
    return this->weapons;
}

void UnitType::setWeapons(const std::vector<Weapon *> &weapons)
{
    if (this->weapons != weapons)
    {
        this->weapons = weapons;
        emit weaponsChanged();
    }
}

QVariantList UnitType::readWeapons() const
{
    return toQVariantList(this->weapons);
}

std::map<WeaponType *, int> UnitType::getAttackSkills() const
{
    return this->attackSkills;
}

void UnitType::setAttackSkills(const std::map<WeaponType *, int> &attackSkills)
{
    if (this->attackSkills != attackSkills)
    {
        this->attackSkills = attackSkills;
        emit attackSkillsChanged();
    }
}

int UnitType::getAttackSkill(WeaponType *weaponType) const
{
    const auto& it = this->attackSkills.find(weaponType);
    int attackSkill{0};

    if (it != this->attackSkills.end())
    {
        attackSkill = it->second;
    }

    return attackSkill;
}

std::map<WeaponClass *, int> UnitType::getDefenseSkills() const
{
    return this->defenseSkills;
}

void UnitType::setDefenseSkills(const std::map<WeaponClass *, int> &defenseSkills)
{
    if (this->defenseSkills != defenseSkills)
    {
        this->defenseSkills = defenseSkills;
        emit defenseSkillsChanged();
    }
}

int UnitType::getDefenseSkill(WeaponClass *weaponClass) const
{
    const auto& it = this->defenseSkills.find(weaponClass);
    int defenseSkill{0};

    if (it != this->defenseSkills.end())
    {
        defenseSkill = it->second;
    }

    return defenseSkill;
}

std::vector<UnitType *> UnitType::getUpgrades() const
{
    return this->upgrades;
}

void UnitType::setUpgrades(const std::vector<UnitType *> &upgrades)
{
    if (this->upgrades != upgrades)
    {
        this->upgrades = upgrades;
        emit upgradesChanged();
    }
}
