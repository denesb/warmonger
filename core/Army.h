#ifndef W_CORE_ARMY_H
#define W_CORE_ARMY_H

#include <vector>

#include "core/ArmyType.h"
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
    Q_PROPERTY(ArmyType* type READ getType WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(MapNode* mapNode READ getMapNode WRITE setMapNode NOTIFY mapNodeChanged)
    Q_PROPERTY(Faction* owner READ getOwner WRITE setOwner NOTIFY ownerChanged)
    Q_PROPERTY(QVariantList units READ readUnits NOTIFY unitsChanged)

public:
    explicit Army(QObject* parent=nullptr);

    QString getDisplayName() const;
    void setDisplayName(const QString& displayName);

    ArmyType* getType() const;
    void setType(ArmyType* type);

    MapNode* getMapNode() const;
    void setMapNode(MapNode *mapNode);

    Faction* getOwner() const;
    void setOwner(Faction *owner);

    std::vector<Unit*> getUnits() const;
    void setUnits(const std::vector<Unit*> &units);
    QVariantList readUnits() const;

signals:
    void displayNameChanged();
    void typeChanged();
    void mapNodeChanged();
    void ownerChanged();
    void unitsChanged();

private:
    QString displayName;
    ArmyType* type;
    MapNode* mapNode;
    Faction* owner;
    std::vector<Unit*> units;
};

} // namespace core
} // namespace warmonger

#endif // W_CORE_ARMY_H
