#ifndef CORE_UNIT_H
#define CORE_UNIT_H

#include <QObject>

#include "core/MapNode.h"
#include "core/Player.h"
#include "core/UnitType.h"

namespace warmonger {
namespace core {

class Unit :
    public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(UnitRank rank READ getRank WRITE setRank NOTIFY rankChanged)
    Q_PROPERTY(QString rankName READ getRankName NOTIFY rankChanged)
    Q_PROPERTY(UnitType *type READ getType WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(MapNode *mapNode READ getMapNode WRITE setMapNode NOTIFY mapNodeChanged)
    Q_PROPERTY(Player *owner READ getOwner WRITE setOwner NOTIFY ownerChanged)
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
    Q_ENUM(UnitRank)

    static const QMap<UnitRank, QString> rank2str;
    static const QMap<UnitRank, QString> rankNames;
    static const QMap<QString, UnitRank> str2rank;

    explicit Unit(QObject *parent=nullptr);
    ~Unit();

    QString getDisplayName() const;
    void setDisplayName(const QString &displayName);

    UnitRank getRank() const;
    void setRank(UnitRank rank);

    QString getRankName() const;

    UnitType * getType() const;
    void setType(UnitType *type);

    MapNode * getMapNode() const;
    void setMapNode(MapNode *mapNode);

    Player * getOwner() const;
    void setOwner(Player *owner);

    double getExperiencePoints() const;
    void setExperiencePoints(double experiencePoints);

    double getHitPoints() const;
    void setHitPoints(double hitPoints);

    double getMovementPoints() const;
    void setMovementPoints(double movementPoints);

signals:
    void displayNameChanged();
    void rankChanged();
    void typeChanged();
    void mapNodeChanged();
    void ownerChanged();
    void experiencePointsChanged();
    void hitPointsChanged();
    void movementPointsChanged();

private:
    void onTypeChanged(const UnitType *oldUnitType);

    QString displayName;
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
