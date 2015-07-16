#ifndef CORE_UNIT_CLASS_H
#define CORE_UNIT_CLASS_H

#include <QMap>

#include "core/GameObject.h"

namespace warmonger {
namespace core {

class TerrainType;

class UnitClass :
    public GameObject
{
    Q_OBJECT

public:
    UnitClass(QObject *parent);
    ~UnitClass();

    int getMovementPoints() const;
    void setMovementPoints(int movementPoints);

    QMap<const TerrainType *, int> getMovements() const;
    void setMovements(const QMap<const TerrainType *, int> &movements);

    int getMovement(const TerrainType *terrainType) const;
    void setMovement(const TerrainType *terrainType, int movement);

    QMap<const TerrainType *, int> getAttacks() const;
    void setAttacks(const QMap<const TerrainType *, int> &attacks);

    int getAttack(const TerrainType *terrainType) const;
    void setAttack(const TerrainType *terrainType, int attack);

    QMap<const TerrainType *, int> getDefenses() const;
    void setDefenses(const QMap<const TerrainType *, int> &defenses);

    int getDefense(const TerrainType *terrainType) const;
    void setDefense(const TerrainType *terrainType, int defense);

private:
    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;

    int movementPoints;
    QMap<const TerrainType *, int> movements;
    QMap<const TerrainType *, int> attacks;
    QMap<const TerrainType *, int> defenses;
};

} // namespace core
} // namespace warmonger

#endif // CORE_UNIT_CLASS_H
