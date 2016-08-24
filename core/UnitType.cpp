#include "core/UnitType.h"
#include "utils/QVariantUtils.h"

namespace warmonger {
namespace core {

UnitType::UnitType(QObject* parent) :
    QObject(parent),
    hitPoints(0),
    experiencePoints(0),
    movementPoints(0),
    recruitmentCost(0),
    upkeepCost(0)
{
}

QString UnitType::getDisplayName() const
{
    return this->displayName;
}

void UnitType::setDisplayName(const QString& displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
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

int UnitType::getExperiencePoints() const
{
    return this->experiencePoints;
}

void UnitType::setExperiencePoints(int experiencePoints)
{
    if (this->experiencePoints != experiencePoints)
    {
        this->experiencePoints = experiencePoints;
        emit experiencePointsChanged();
    }
}

int UnitType::getMovementPoints() const
{
    return this->movementPoints;
}

void UnitType::setMovementPoints(int movementPoints)
{
    if (this->movementPoints != movementPoints)
    {
        this->movementPoints = movementPoints;
        emit movementPointsChanged();
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

const std::map<TerrainType*, int>& UnitType::getMovementCosts() const
{
    return this->movementCosts;
}

void UnitType::setMovementCosts(const std::map<TerrainType*, int>& movementCosts)
{
    this->movementCosts = movementCosts;
}

QVariantMap UnitType::readMovementCosts() const
{
    return utils::toQVariantMap(
            this->movementCosts,
            utils::qObjectName,
            utils::verbatim<int>);
}

int UnitType::getMovementCost(TerrainType* terrainType) const
{
    return this->movementCosts.at(terrainType);
}

void UnitType::setMovementCost(TerrainType* terrainType, int movement)
{
    this->movementCosts[terrainType] = movement;
}

const std::vector<UnitType*>& UnitType::getUpgrades() const
{
    return this->upgrades;
}

void UnitType::setUpgrades(const std::vector<UnitType*>& upgrades)
{
    if (this->upgrades != upgrades)
    {
        this->upgrades = upgrades;
        emit upgradesChanged();
    }
}

QVariantList UnitType::readUpgrades() const
{
    return utils::toQVariantList(this->upgrades);
}

} // namespace core
} // namespace warmonger
