#ifndef CORE_SETTLEMENT_H
#define CORE_SETTLEMENT_H

#include <QPoint>

#include "core/GameObject.h"
#include "core/MapNode.h"
#include "core/Player.h"
#include "core/SettlementType.h"

namespace warmonger {
namespace core {

class Settlement :
    public GameObject
{
    Q_OBJECT
    Q_PROPERTY(SettlementType *type READ getType WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(MapNode *mapNode READ getMapNode WRITE setMapNode NOTIFY mapNodeChanged)
    Q_PROPERTY(Player *owner READ getOwner WRITE setOwner NOTIFY ownerChanged)

public:
    Settlement(QObject *parent);
    ~Settlement();

    SettlementType * getType() const;
    void setType(SettlementType *type);

    MapNode * getMapNode() const;
    void setMapNode(MapNode *mapNode);

    Player * getOwner() const;
    void setOwner(Player *owner);

signals:
    void typeChanged();
    void mapNodeChanged();
    void ownerChanged();

private:
    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;

    SettlementType *type;
    MapNode *mapNode;
    Player *owner;
};

} // namespace core
} // namespace warmonger

#endif // CORE_SETTLEMENT_H
