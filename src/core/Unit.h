#ifndef CORE_UNIT_H
#define CORE_UNIT_H

#include <QPoint>

#include "core/GameObject.h"

namespace warmonger {
namespace core {

class UnitType;

class Unit :
    public GameObject
{
    Q_OBJECT

public:
    Unit(QObject *parent = nullptr);
    ~Unit();

    UnitType * getUnitType() const;
    void setUnitType(UnitType *unitType);

    QPoint getPosition() const;
    void setPosition(const QPoint &position);

    int getHitpoints() const;
    void setHitpoints(int hitPoints);

    int getMovementpoints() const;
    void setMovementpoints(int movementPoints);

    int getExperience() const;
    void setExperience(int experience);

    void fromJson(const QJsonObject &obj);
    QJsonObject toJson() const;

private:
    UnitType *unitType;
    QPoint position;
    int hitPoints;
    int movementPoints;
    int experience;
};

}; // namespace core
}; // namespace warmonger

#endif // CORE_UNIT_H
