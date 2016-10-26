#include "core/World.h"
#include "utils/QVariantUtils.h"

namespace warmonger {
namespace core {

World::World(QObject* parent)
    : QObject(parent)
{
}

void World::setDisplayName(const QString& displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
}

void World::setArmyTypes(const std::vector<ArmyType*>& armyTypes)
{
    if (this->armyTypes != armyTypes)
    {
        this->armyTypes = armyTypes;
        emit armyTypesChanged();
    }
}

QVariantList World::readArmyTypes() const
{
    return utils::toQVariantList(this->armyTypes);
}

void World::setBanners(const std::vector<Banner*>& banners)
{
    if (this->banners != banners)
    {
        this->banners = banners;
        emit bannersChanged();
    }
}

QVariantList World::readBanners() const
{
    return utils::toQVariantList(this->banners);
}

void World::setCivilizations(const std::vector<Civilization*>& civilizations)
{
    this->civilizations = civilizations;
}

QVariantList World::readCivilizations() const
{
    return utils::toQVariantList(this->civilizations);
}

void World::setSettlementTypes(const std::vector<SettlementType*>& settlementTypes)
{
    if (this->settlementTypes != settlementTypes)
    {
        this->settlementTypes = settlementTypes;
        emit settlementTypesChanged();
    }
}

QVariantList World::readSettlementTypes() const
{
    return utils::toQVariantList(this->settlementTypes);
}

void World::setTerrainTypes(const std::vector<TerrainType*>& terrainTypes)
{
    if (this->terrainTypes != terrainTypes)
    {
        this->terrainTypes = terrainTypes;
        emit terrainTypesChanged();
    }
}

QVariantList World::readTerrainTypes() const
{
    return utils::toQVariantList(this->terrainTypes);
}

void World::setUnitTypes(const std::vector<UnitType*>& unitTypes)
{
    if (this->unitTypes != unitTypes)
    {
        this->unitTypes = unitTypes;
        emit unitTypesChanged();
    }
}

QVariantList World::readUnitTypes() const
{
    return utils::toQVariantList(this->unitTypes);
}

} // namespace core
} // namespace warmonger
