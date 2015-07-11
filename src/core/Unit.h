#ifndef CORE_UNIT_H
#define CORE_UNIT_H

#include <QPoint>

#include "core/GameObject.h"
#include "core/MapPosition.h"

namespace warmonger {
namespace core {

class UnitType;

class Unit :
    public GameObject
{
    Q_OBJECT

public:
    Unit(QObject *parent);
    ~Unit();

    const UnitType * getUnitType() const;
    void setUnitType(const UnitType *unitType);

    MapPosition getPosition() const;
    void setPosition(const MapPosition &position);

    int getHitpoints() const;
    void setHitpoints(int hitPoints);

    int getMovementpoints() const;
    void setMovementpoints(int movementPoints);

    int getExperience() const;
    void setExperience(int experience);

    void fromJson(const QJsonObject &obj);
    QJsonObject toJson() const;

private:
    const UnitType *unitType;
    MapPosition position;
    int hitPoints;
    int movementPoints;
    int experience;
};

}; // namespace core
}; // namespace warmonger

#endif // CORE_UNIT_H
