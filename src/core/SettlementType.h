#ifndef CORE_SETTLEMENT_TYPE_H
#define CORE_SETTLEMENT_TYPE_H

#include "core/WorldItem.h"

namespace warmonger {
namespace core {

class UnitType;

class SettlementType :
    public WorldItem
{
    Q_OBJECT;

public:
    SettlementType(QObject *parent);
    ~SettlementType();

    int getGoldPerTurn() const;
    void setGoldPerTurn(int goldPerTurn);

    QList<const UnitType *> getRecruits() const;
    void setRecruits(const QList<const UnitType *> &recruits);

    void fromJson(const QJsonObject &obj);
    QJsonObject toJson() const;

private:
    int goldPerTurn;
    QList<const UnitType *> recruits;
};

}; // namespace core
}; // namespace warmonger

#endif // CORE_SETTLEMENT_TYPE_H
