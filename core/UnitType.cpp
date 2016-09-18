#include "core/UnitType.h"
#include "utils/QVariantUtils.h"

namespace warmonger {
namespace core {

UnitType::UnitType(QObject* parent)
    : QObject(parent)
{
}

QString UnitType::getDisplayName() const
{
    if (this->displayName)
        return *this->displayName;
    else if (this->isHierarchyRoot())
        throw utils::ValueError("displayName is unset");
    else
        return this->getHierarchyParent()->getDisplayName();
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
    if (this->hitPoints)
        return *this->hitPoints;
    else if (this->isHierarchyRoot())
        throw utils::ValueError("hitPoints is unset");
    else
        return this->getHierarchyParent()->getHitPoints();
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
    if (this->experiencePoints)
        return *this->experiencePoints;
    else if (this->isHierarchyRoot())
        throw utils::ValueError("experiencePoints is unset");
    else
        return this->getHierarchyParent()->getExperiencePoints();
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
    if (this->movementPoints)
        return *this->movementPoints;
    else if (this->isHierarchyRoot())
        throw utils::ValueError("movementPoints is unset");
    else
        return this->getHierarchyParent()->getMovementPoints();
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
    if (this->recruitmentCost)
        return *this->recruitmentCost;
    else if (this->isHierarchyRoot())
        throw utils::ValueError("recruitmentCost is unset");
    else
        return this->getHierarchyParent()->getRecruitmentCost();
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
    if (this->upkeepCost)
        return *this->upkeepCost;
    else if (this->isHierarchyRoot())
        throw utils::ValueError("upkeepCost is unset");
    else
        return this->getHierarchyParent()->getUpkeepCost();
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
    if (this->movementCosts)
        return *this->movementCosts;
    else if (this->isHierarchyRoot())
        throw utils::ValueError("movementCosts is unset");
    else
        return this->getHierarchyParent()->getMovementCosts();
}

void UnitType::setMovementCosts(const std::map<TerrainType*, int>& movementCosts)
{
    this->movementCosts = movementCosts;
}

QVariantMap UnitType::readMovementCosts() const
{
    return utils::toQVariantMap(this->getMovementCosts(), utils::qObjectName, utils::verbatim<int>);
}

int UnitType::getMovementCost(TerrainType* terrainType) const
{
    return this->getMovementCosts().at(terrainType);
}

void UnitType::setMovementCost(TerrainType* terrainType, int movement)
{
    if (!this->movementCosts)
        this->movementCosts = std::map<TerrainType*, int>();

    (*this->movementCosts)[terrainType] = movement;
}

const std::vector<UnitType*>& UnitType::getUpgrades() const
{
    if (this->upgrades)
        return *this->upgrades;
    else if (this->isHierarchyRoot())
        throw utils::ValueError("upgrades is unset");
    else
        return this->getHierarchyParent()->getUpgrades();
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
    return utils::toQVariantList(this->getUpgrades());
}

} // namespace core
} // namespace warmonger
