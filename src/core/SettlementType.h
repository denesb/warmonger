#ifndef CORE_SETTLEMENT_TYPE_H
#define CORE_SETTLEMENT_TYPE_H

#include "core/GameObject.h"

namespace warmonger {
namespace core {

class UnitType;

class SettlementType :
    public GameObject
{
    Q_OBJECT;
    Q_PROPERTY(int goldPerTurn READ getGoldPerTurn WRITE setGoldPerTurn NOTIFY goldPerTurnChanged)
    Q_PROPERTY(QVariantList recruits READ readRecruits WRITE writeRecruits NOTIFY recruitsChanged)

public:
    SettlementType(QObject *parent);
    ~SettlementType();

    int getGoldPerTurn() const;
    void setGoldPerTurn(int goldPerTurn);

    QList<UnitType *> getRecruits() const;
    void setRecruits(const QList<UnitType *> &recruits);
    QVariantList readRecruits() const;
    void writeRecruits(QVariantList recruits);

signals:
    void goldPerTurnChanged();
    void recruitsChanged();

private:
    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;

    int goldPerTurn;
    QList<UnitType *> recruits;
};

} // namespace core
} // namespace warmonger

#endif // CORE_SETTLEMENT_TYPE_H
