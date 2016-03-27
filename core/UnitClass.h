#ifndef CORE_UNIT_CLASS_H
#define CORE_UNIT_CLASS_H

#include <map>

#include <QObject>

#include "core/TerrainType.h"

namespace warmonger {
namespace core {

class UnitClass :
    public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(int movementPoints READ getMovementPoints WRITE setMovementPoints NOTIFY movementPointsChanged)

public:
    explicit UnitClass(QObject *parent=nullptr);
    ~UnitClass();

    QString getDisplayName() const;
    void setDisplayName(const QString &displayName);

    int getMovementPoints() const;
    void setMovementPoints(int movementPoints);

    std::map<const TerrainType *, int> getMovementCosts() const;
    void setMovementCosts(const std::map<const TerrainType *, int> &movementCosts);

    int getMovementCost(const TerrainType *terrainType) const;
    void setMovementCost(const TerrainType *terrainType, int movementCost);

    std::map<const TerrainType *, int> getAttacks() const;
    void setAttacks(const std::map<const TerrainType *, int> &attacks);

    int getAttack(const TerrainType *terrainType) const;
    void setAttack(const TerrainType *terrainType, int attack);

    std::map<const TerrainType *, int> getDefenses() const;
    void setDefenses(const std::map<const TerrainType *, int> &defenses);

    int getDefense(const TerrainType *terrainType) const;
    void setDefense(const TerrainType *terrainType, int defense);

signals:
    void displayNameChanged();
    void movementPointsChanged();

private:
    QString displayName;
    int movementPoints;
    std::map<const TerrainType *, int> movementCosts;
    std::map<const TerrainType *, int> attacks;
    std::map<const TerrainType *, int> defenses;
};

} // namespace core
} // namespace warmonger

#endif // CORE_UNIT_CLASS_H
