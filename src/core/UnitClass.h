#ifndef CORE_UNIT_CLASS_H
#define CORE_UNIT_CLASS_H

#include <QMap>

#include "core/TerrainType.h"

namespace core {

class UnitClass :
    public WorldItem
{
public:
    UnitClass(QObject *parent = nullptr);
    ~UnitClass();

    int getMovementPoints() const;
    void setMovementPoints(int movementPoints);

    QMap<TerrainType *, int> getMovements() const;
    void setMovements(const QMap<TerrainType *, int> &movements);

    int getMovement(TerrainType *terrainType) const;
    void setMovement(TerrainType *terrainType, int movement);

    QMap<TerrainType *, int> getAttacks() const;
    void setAttacks(const QMap<TerrainType *, int> &attacks);

    int getAttack(TerrainType *terrainType) const;
    void setAttack(TerrainType *terrainType, int attack);

    QMap<TerrainType *, int> getDefenses() const;
    void setDefenses(const QMap<TerrainType *, int> &defenses);

    int getDefense(TerrainType *terrainType) const;
    void setDefense(TerrainType *terrainType, int defense);

    void fromJson(const QJsonObject &json);
    QJsonObject toJson() const;

private:
    int movementPoints;
    QMap<TerrainType *, int> movements;
    QMap<TerrainType *, int> attacks;
    QMap<TerrainType *, int> defenses;
};

}; // namespace core

#endif // CORE_UNIT_CLASS_H
