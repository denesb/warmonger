#ifndef CORE_SETTLEMENT_H
#define CORE_SETTLEMENT_H

#include <vector>

#include <QObject>

#include "core/Faction.h"
#include "core/MapNode.h"
#include "core/SettlementType.h"

namespace warmonger {
namespace core {

class Settlement : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(SettlementType* type READ getType WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(MapNode* mapNode READ getMapNode WRITE setMapNode NOTIFY mapNodeChanged)
    Q_PROPERTY(Faction* owner READ getOwner WRITE setOwner NOTIFY ownerChanged)

public:
    explicit Settlement(QObject* parent = nullptr);
    ~Settlement();

    QString getDisplayName() const;
    void setDisplayName(const QString& displayName);

    SettlementType* getType() const;
    void setType(SettlementType* type);

    MapNode* getMapNode() const;
    void setMapNode(MapNode* mapNode);

    Faction* getOwner() const;
    void setOwner(Faction* owner);

signals:
    void displayNameChanged();
    void typeChanged();
    void mapNodeChanged();
    void ownerChanged();

private:
    QString displayName;
    SettlementType* type;
    MapNode* mapNode;
    Faction* owner;
};

} // namespace core
} // namespace warmonger

#endif // CORE_SETTLEMENT_H
