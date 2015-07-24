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

public:
    Settlement(QObject *parent);
    ~Settlement();

    const SettlementType * getSettlementType() const;
    void setSettlementType(const SettlementType *settlementType);

    const MapNode * getMapNode() const;
    void setMapNode(const MapNode *mapNode);

private:
    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;

    const SettlementType *settlementType;
    const MapNode *mapNode;
};

} // namespace core
} // namespace warmonger

#endif // CORE_SETTLEMENT_H
