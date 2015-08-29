#ifndef CORE_SETTLEMENT_H
#define CORE_SETTLEMENT_H

#include <QPoint>

#include "core/GameObject.h"

namespace warmonger {
namespace core {

class SettlementType;
class MapNode;

class Settlement :
    public GameObject
{
    Q_OBJECT
    Q_PROPERTY(QObject * settlementType READ readSettlementType WRITE writeSettlementType NOTIFY settlementTypeChanged)
    Q_PROPERTY(QObject * mapNode READ readMapNode WRITE writeMapNode NOTIFY mapNodeChanged)

public:
    Settlement(QObject *parent);
    ~Settlement();

    SettlementType * getSettlementType() const;
    void setSettlementType(SettlementType *settlementType);
    QObject * readSettlementType() const;
    void writeSettlementType(QObject *settlementType);

    MapNode * getMapNode() const;
    void setMapNode(MapNode *mapNode);
    QObject * readMapNode() const;
    void writeMapNode(QObject *mapNode);

signals:
    void settlementTypeChanged();
    void mapNodeChanged();

private:
    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;

    SettlementType *settlementType;
    MapNode *mapNode;
};

} // namespace core
} // namespace warmonger

#endif // CORE_SETTLEMENT_H
