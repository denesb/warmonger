#ifndef W_CORE_UNIT_TYPE_H
#define W_CORE_UNIT_TYPE_H

#include <map>
#include <vector>

#include <boost/optional.hpp>

#include <QObject>
#include <QVariant>

#include "core/HierarchyNode.hpp"
#include "core/TerrainType.h"

namespace warmonger {
namespace core {

/**
 * Defines a type of or kind of unit.
 *
 * Can inherit:
 * * displayName
 * * hitPoints
 * * experiencePoints
 * * movementPoints
 * * recruitmentCost
 * * upkeepCost
 * * movementCosts
 * * upgrades
 */
class UnitType : public QObject, public HierarchyNode<UnitType>
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(int hitPoints READ getHitPoints NOTIFY hitPointsChanged)
    Q_PROPERTY(int experiencePoints READ getExperiencePoints NOTIFY experiencePointsChanged)
    Q_PROPERTY(int movementPoints READ getMovementPoints NOTIFY movementPointsChanged)
    Q_PROPERTY(int recruitmentCost READ getRecruitmentCost NOTIFY recruitmentCostChanged)
    Q_PROPERTY(int upkeepCost READ getRecruitmentCost NOTIFY upkeepCostChanged)
    Q_PROPERTY(QVariantMap movementCosts READ readMovementCosts NOTIFY movementCostsChanged)
    Q_PROPERTY(QVariantList upgrades READ readUpgrades NOTIFY upgradesChanged)

public:
    explicit UnitType(QObject* parent = nullptr);

    QString getDisplayName() const;
    void setDisplayName(const QString& displayName);

    int getHitPoints() const;
    void setHitPoints(int hitPoints);

    int getExperiencePoints() const;
    void setExperiencePoints(int experiencePoints);

    int getMovementPoints() const;
    void setMovementPoints(int movementPoints);

    int getRecruitmentCost() const;
    void setRecruitmentCost(int recruitmentCost);

    int getUpkeepCost() const;
    void setUpkeepCost(int upkeepCost);

    const std::map<TerrainType*, int>& getMovementCosts() const;
    void setMovementCosts(const std::map<TerrainType*, int>& movementCosts);

    QVariantMap readMovementCosts() const;

    int getMovementCost(TerrainType* terrainType) const;
    void setMovementCost(TerrainType* terrainType, int movementCost);

    const std::vector<UnitType*>& getUpgrades() const;
    void setUpgrades(const std::vector<UnitType*>& upgrades);

    QVariantList readUpgrades() const;

signals:
    void displayNameChanged();
    void hitPointsChanged();
    void experiencePointsChanged();
    void movementPointsChanged();
    void recruitmentCostChanged();
    void upkeepCostChanged();
    void movementCostsChanged();
    void upgradesChanged();

private:
    boost::optional<QString> displayName;
    boost::optional<int> hitPoints;
    boost::optional<int> experiencePoints;
    boost::optional<int> movementPoints;
    boost::optional<int> recruitmentCost;
    boost::optional<int> upkeepCost;
    boost::optional<std::map<TerrainType*, int>> movementCosts;
    boost::optional<std::vector<UnitType*>> upgrades;
};

} // namespace core
} // namespace warmonger

#endif // W_CORE_UNIT_TYPE_H
