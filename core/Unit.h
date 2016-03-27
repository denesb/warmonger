#ifndef CORE_UNIT_H
#define CORE_UNIT_H

#include <QObject>

#include "core/MapNode.h"
#include "core/Faction.h"
#include "core/UnitType.h"

namespace warmonger {
namespace core {

class Unit :
    public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(UnitType *type READ getType WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(MapNode *mapNode READ getMapNode WRITE setMapNode NOTIFY mapNodeChanged)
    Q_PROPERTY(Faction *owner READ getOwner WRITE setOwner NOTIFY ownerChanged)
    Q_PROPERTY(int experiencePoints READ getExperiencePoints WRITE setExperiencePoints NOTIFY experiencePointsChanged)
    Q_PROPERTY(int hitPoints READ getHitPoints WRITE setHitPoints NOTIFY hitPointsChanged)
    Q_PROPERTY(int movementPoints READ getMovementPoints WRITE setMovementPoints NOTIFY movementPointsChanged)

public:
    explicit Unit(QObject *parent=nullptr);
    ~Unit();

    QString getDisplayName() const;
    void setDisplayName(const QString &displayName);

    UnitType * getType() const;
    void setType(UnitType *type);

    MapNode * getMapNode() const;
    void setMapNode(MapNode *mapNode);

    Faction * getOwner() const;
    void setOwner(Faction *owner);

    int getExperiencePoints() const;
    void setExperiencePoints(int experiencePoints);

    int getHitPoints() const;
    void setHitPoints(int hitPoints);

    int getMovementPoints() const;
    void setMovementPoints(int movementPoints);

signals:
    void displayNameChanged();
    void typeChanged();
    void mapNodeChanged();
    void ownerChanged();
    void experiencePointsChanged();
    void hitPointsChanged();
    void movementPointsChanged();

private:
    void onTypeChanged(const UnitType *oldUnitType);

    QString displayName;
    UnitType *type;
    MapNode *mapNode;
    Faction *owner;
    int experiencePoints;
    int hitPoints;
    int movementPoints;
};

} // namespace core
} // namespace warmonger

#endif // CORE_UNIT_H
