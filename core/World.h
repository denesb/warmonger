#ifndef W_CORE_WORLD_H
#define W_CORE_WORLD_H

#include <vector>

#include <QObject>
#include <QVariant>

#include "core/ArmyType.h"
#include "core/Civilization.h"
#include "core/SettlementType.h"
#include "core/TerrainType.h"
#include "core/UnitType.h"

namespace warmonger {
namespace core {

class World :
    public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(QVariantList armyTypes READ readArmyTypes NOTIFY armyTypesChanged)
    Q_PROPERTY(QVariantList civilizations READ readCivilizations NOTIFY civilizationsChanged)
    Q_PROPERTY(QVariantList settlementTypes READ readSettlementTypes NOTIFY settlementTypesChanged)
    Q_PROPERTY(QVariantList terrainTypes READ readTerrainTypes NOTIFY terrainTypesChanged)
    Q_PROPERTY(QVariantList unitTypes READ readUnitTypes NOTIFY unitTypesChanged)

public:
    explicit World(QObject*parent=nullptr);

    QString getDisplayName() const;
    void setDisplayName(const QString& displayName);

    std::vector<ArmyType*> getArmyTypes() const;
    void setArmyTypes(const std::vector<ArmyType*>& armyTpes);
    QVariantList readArmyTypes() const;

    std::vector<Civilization*> getCivilizations() const;
    void setCivilizations(const std::vector<Civilization*>& civilizations);
    QVariantList readCivilizations() const;

    std::vector<SettlementType*> getSettlementTypes() const;
    void setSettlementTypes(const std::vector<SettlementType*>& settlementTypes);
    QVariantList readSettlementTypes() const;

    std::vector<TerrainType*> getTerrainTypes() const;
    void setTerrainTypes(const std::vector<TerrainType*>& terrainTypes);
    QVariantList readTerrainTypes() const;

    std::vector<UnitType*> getUnitTypes() const;
    void setUnitTypes(const std::vector<UnitType*>& unitTypes);
    QVariantList readUnitTypes() const;

signals:
    void displayNameChanged();
    void armyTypesChanged();
    void terrainTypesChanged();
    void unitTypesChanged();
    void settlementTypesChanged();
    void civilizationsChanged();

private:
    QString displayName;
    std::vector<ArmyType*> armyTypes;
    std::vector<TerrainType*> terrainTypes;
    std::vector<UnitType*> unitTypes;
    std::vector<SettlementType*> settlementTypes;
    std::vector<Civilization*> civilizations;
};

} // namespace core
} // namespace warmonger

#endif // W_CORE_WORLD_H
