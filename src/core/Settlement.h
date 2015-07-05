#ifndef CORE_SETTLEMENT_H
#define CORE_SETTLEMENT_H

#include <QPoint>

#include "core/GameObject.h"

namespace warmonger {
namespace core {

class SettlementType;

class Settlement :
    public GameObject
{
    Q_OBJECT

public:
    Settlement(QObject *parent = nullptr);
    ~Settlement();

    QPoint getPosition() const;
    void setPosition(const QPoint &position);

    void fromJson(const QJsonObject &obj);
    QJsonObject toJson() const;

private:
    SettlementType *settlementType;
    QPoint position;
};

}; // namespace core
}; // namespace warmonger

#endif // CORE_SETTLEMENT_H
