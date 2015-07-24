#ifndef CORE_UNIT_H
#define CORE_UNIT_H

#include <QPoint>

#include "core/GameObject.h"

namespace warmonger {
namespace core {

class UnitType;
class MapNode;

class Unit :
    public GameObject
{
    Q_OBJECT

public:
    Unit(QObject *parent);
    ~Unit();

    const UnitType * getUnitType() const;
    void setUnitType(const UnitType *unitType);

    const MapNode * getMapNode() const;
    void setMapNode(const MapNode *mapNode);

    int getHitpoints() const;
    void setHitpoints(int hitPoints);

    int getMovementpoints() const;
    void setMovementpoints(int movementPoints);

    int getExperience() const;
    void setExperience(int experience);

private:
    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;

    const UnitType *unitType;
    const MapNode * mapNode;
    int hitPoints;
    int movementPoints;
    int experience;
};

} // namespace core
} // namespace warmonger

#endif // CORE_UNIT_H
