#ifndef CORE_UNIT_H
#define CORE_UNIT_H

#include <QPoint>

#include "core/GameObject.h"

namespace warmonger {
namespace core {

class UnitType;
class MapNode;
class Player;

class Unit :
    public GameObject
{
    Q_OBJECT
    Q_PROPERTY(QObject * type READ readType WRITE writeType NOTIFY typeChanged)
    Q_PROPERTY(QObject * mapNode READ readMapNode WRITE writeMapNode NOTIFY mapNodeChanged)
    Q_PROPERTY(QObject * owner READ readOwner WRITE writeOwner NOTIFY ownerChanged)
    Q_PROPERTY(int experiencePoints READ getExperiencePoints WRITE setExperiencePoints NOTIFY experiencePointsChanged)
    Q_PROPERTY(int hitPoints READ getHitPoints WRITE setHitPoints NOTIFY hitPointsChanged)
    Q_PROPERTY(int movementPoints READ getMovementPoints WRITE setMovementPoints NOTIFY movementPointsChanged)

public:
    enum UnitRank
    {
        Soldier = 0,
        Officer = 1,
        Leader = 2
    };

    static const QMap<UnitRank, QString> rank2str;
    static const QMap<QString, UnitRank> str2rank;

    Unit(QObject *parent);
    ~Unit();

    UnitRank getRank() const;
    void setRank(UnitRank rank);

    UnitType * getType() const;
    void setType(UnitType *type);
    QObject * readType() const;
    void writeType(QObject *type);

    MapNode * getMapNode() const;
    QObject * readMapNode() const;
    void writeMapNode(QObject *mapNode);
    void setMapNode(MapNode *mapNode);

    Player * getOwner() const;
    void setOwner(Player *owner);
    QObject * readOwner() const;
    void writeOwner(QObject *owner);

    double getExperiencePoints() const;
    void setExperiencePoints(double experiencePoints);

    double getHitPoints() const;
    void setHitPoints(double hitPoints);

    double getMovementPoints() const;
    void setMovementPoints(double movementPoints);

signals:
    void typeChanged();
    void mapNodeChanged();
    void ownerChanged();
    void experiencePointsChanged();
    void hitPointsChanged();
    void movementPointsChanged();

private:
    void onTypeChanged(const UnitType *oldUnitType);

    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;

    UnitRank rank;
    UnitType *type;
    MapNode *mapNode;
    Player *owner;
    double experiencePoints;
    double hitPoints;
    double movementPoints;
};

} // namespace core
} // namespace warmonger

#endif // CORE_UNIT_H
