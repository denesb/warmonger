#ifndef CORE_SETTLEMENT_H
#define CORE_SETTLEMENT_H

#include <QPoint>

#include "core/GameObject.h"
#include "core/MapPosition.h"

namespace warmonger {
namespace core {

class SettlementType;

class Settlement :
    public GameObject
{
    Q_OBJECT

public:
    Settlement(QObject *parent);
    ~Settlement();

    const SettlementType * getSettlementType() const;
    void setSettlementType(const SettlementType *settlementType);

    MapPosition getPosition() const;
    void setPosition(const MapPosition &position);

private:
    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;

    const SettlementType *settlementType;
    MapPosition position;
};

} // namespace core
} // namespace warmonger

#endif // CORE_SETTLEMENT_H
