#ifndef W_CORE_WORLD_H
#define W_CORE_WORLD_H

#include <vector>

#include <QObject>
#include <QVariant>

#include "core/ArmyType.h"
#include "core/Banner.h"
#include "core/Civilization.h"
#include "core/SettlementType.h"
#include "core/TerrainType.h"
#include "core/UnitType.h"

namespace warmonger {
namespace core {

/**
 * A Warmonger World.
 */
class World : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(QVariantList armyTypes READ readArmyTypes NOTIFY armyTypesChanged)
    Q_PROPERTY(QVariantList banners READ readBanners NOTIFY bannersChanged)
    Q_PROPERTY(QVariantList civilizations READ readCivilizations NOTIFY civilizationsChanged)
    Q_PROPERTY(QVariantList settlementTypes READ readSettlementTypes NOTIFY settlementTypesChanged)
    Q_PROPERTY(QVariantList terrainTypes READ readTerrainTypes NOTIFY terrainTypesChanged)
    Q_PROPERTY(QVariantList unitTypes READ readUnitTypes NOTIFY unitTypesChanged)

public:
    explicit World(QObject* parent = nullptr);

    const QString& getDisplayName() const
    {
        return this->displayName;
    }

    void setDisplayName(const QString& displayName);

    const std::vector<ArmyType*>& getArmyTypes() const
    {
        return this->armyTypes;
    }

    void setArmyTypes(const std::vector<ArmyType*>& armyTpes);
    QVariantList readArmyTypes() const;

    const std::vector<Banner*>& getBanners() const
    {
        return this->banners;
    }

    QVariantList readBanners() const;
    void setBanners(const std::vector<Banner*>& banners);

    const std::vector<Civilization*>& getCivilizations() const
    {
        return this->civilizations;
    }

    void setCivilizations(const std::vector<Civilization*>& civilizations);
    QVariantList readCivilizations() const;

    const std::vector<SettlementType*>& getSettlementTypes() const
    {
        return this->settlementTypes;
    }

    void setSettlementTypes(const std::vector<SettlementType*>& settlementTypes);
    QVariantList readSettlementTypes() const;

    const std::vector<TerrainType*>& getTerrainTypes() const
    {
        return this->terrainTypes;
    }

    void setTerrainTypes(const std::vector<TerrainType*>& terrainTypes);
    QVariantList readTerrainTypes() const;

    const std::vector<UnitType*>& getUnitTypes() const
    {
        return this->unitTypes;
    }

    void setUnitTypes(const std::vector<UnitType*>& unitTypes);
    QVariantList readUnitTypes() const;

signals:
    void displayNameChanged();
    void armyTypesChanged();
    void bannersChanged();
    void civilizationsChanged();
    void terrainTypesChanged();
    void unitTypesChanged();
    void settlementTypesChanged();

private:
    QString displayName;
    std::vector<ArmyType*> armyTypes;
    std::vector<Banner*> banners;
    std::vector<Civilization*> civilizations;
    std::vector<TerrainType*> terrainTypes;
    std::vector<UnitType*> unitTypes;
    std::vector<SettlementType*> settlementTypes;
};

} // namespace core
} // namespace warmonger

#endif // W_CORE_WORLD_H
