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
    SettlementType(QObject *parent = nullptr);
    ~SettlementType();

    int getGoldPerTurn() const;
    void setGoldPerTurn(int goldPerTurn);

    QList<UnitType *> getRecruits() const;
    void setRecruits(const QList<UnitType *> &recruits);

    void fromJson(const QJsonObject &obj);
    QJsonObject toJson() const;

private:
    int goldPerTurn;
    QList<UnitType *> recruits;
};

}; // namespace core
}; // namespace warmonger

#endif // CORE_SETTLEMENT_TYPE_H
