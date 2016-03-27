#ifndef CORE_ARMY_H
#define CORE_ARMY_H

#include <vector>

#include "core/Faction.h"
#include "core/MapNode.h"
#include "core/Unit.h"

namespace warmonger {
namespace core {

class Army :
    public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(MapNode *mapNode READ getMapNode WRITE setMapNode NOTIFY mapNodeChanged)
    Q_PROPERTY(Faction *owner READ getOwner WRITE setOwner NOTIFY ownerChanged)
    Q_PROPERTY(QVariantList units READ readUnits NOTIFY unitsChanged)

public:
    explicit Army(QObject *parent=nullptr);

    QString getDisplayName() const;
    void setDisplayName(const QString &displayName);

    MapNode * getMapNode() const;
    void setMapNode(MapNode *mapNode);

    Faction * getOwner() const;
    void setOwner(Faction *owner);

    std::vector<Unit *> getUnits() const;
    void setUnits(const std::vector<Unit *> &units);
    QVariantList readUnits() const;

signals:
    void displayNameChanged();
    void mapNodeChanged();
    void ownerChanged();
    void unitsChanged();

private:
    QString displayName;
    MapNode *mapNode;
    Faction *owner;
    std::vector<Unit *> units;
};

} // namespace core
} // namespace warmonger

#endif // CORE_ARMY_H
