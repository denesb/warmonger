#ifndef CORE_UNIT_CLASS_H
#define CORE_UNIT_CLASS_H

#include <QObject>
#include <QMap>

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

    QMap<const TerrainType *, int> getMovementCosts() const;
    void setMovementCosts(const QMap<const TerrainType *, int> &movementCosts);

    int getMovementCost(const TerrainType *terrainType) const;
    void setMovementCost(const TerrainType *terrainType, int movementCost);

    QMap<const TerrainType *, int> getAttacks() const;
    void setAttacks(const QMap<const TerrainType *, int> &attacks);

    int getAttack(const TerrainType *terrainType) const;
    void setAttack(const TerrainType *terrainType, int attack);

    QMap<const TerrainType *, int> getDefenses() const;
    void setDefenses(const QMap<const TerrainType *, int> &defenses);

    int getDefense(const TerrainType *terrainType) const;
    void setDefense(const TerrainType *terrainType, int defense);

signals:
    void displayNameChanged();
    void movementPointsChanged();

private:
    QString displayName;
    int movementPoints;
    QMap<const TerrainType *, int> movementCosts;
    QMap<const TerrainType *, int> attacks;
    QMap<const TerrainType *, int> defenses;
};

} // namespace core
} // namespace warmonger

#endif // CORE_UNIT_CLASS_H
