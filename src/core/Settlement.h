#ifndef CORE_SETTLEMENT_H
#define CORE_SETTLEMENT_H

#include <QPoint>

#include "core/GameObject.h"

namespace warmonger {
namespace core {

class SettlementType;
class MapNode;
class Player;

class Settlement :
    public GameObject
{
    Q_OBJECT
    Q_PROPERTY(QObject * type READ readType WRITE writeType NOTIFY typeChanged)
    Q_PROPERTY(QObject * mapNode READ readMapNode WRITE writeMapNode NOTIFY mapNodeChanged)
    Q_PROPERTY(QObject * owner READ readOwner WRITE writeOwner NOTIFY ownerChanged)

public:
    Settlement(QObject *parent);
    ~Settlement();

    SettlementType * getType() const;
    void setType(SettlementType *type);
    QObject * readType() const;
    void writeType(QObject *type);

    MapNode * getMapNode() const;
    void setMapNode(MapNode *mapNode);
    QObject * readMapNode() const;
    void writeMapNode(QObject *mapNode);

    Player * getOwner() const;
    void setOwner(Player *owner);
    QObject * readOwner() const;
    void writeOwner(QObject *owner);

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
