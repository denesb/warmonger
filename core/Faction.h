#ifndef CORE_FACTION_H
#define CORE_FACTION_H

#include "core/GameObject.h"

namespace warmonger {
namespace core {

class UnitType;
class SettlementType;

class Faction :
    public GameObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList unitTypes READ readUnitTypes NOTIFY unitTypesChanged)
    Q_PROPERTY(QVariantMap recruits READ readRecruits NOTIFY recruitsChanged)

public:
    Faction(QObject *parent);
    ~Faction();

    QList<UnitType *> getUnitTypes() const;
    QVariantList readUnitTypes() const;
    void setUnitTypes(const QList<UnitType *> &unitTypes);

    QMap<SettlementType *, QList<UnitType *>> getRecruits() const;
    QVariantMap readRecruits() const;
    void setRecruits(const QMap<SettlementType *, QList<UnitType *>> &recruits);

    QList<UnitType *> getRecruitsFor(
        SettlementType *settlementType
    ) const;
    bool canRecruitFrom(
        SettlementType *settlemntType,
        UnitType *unitType
    ) const;

signals:
    void unitTypesChanged();
    void recruitsChanged();

private:
    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;

    QList<UnitType *> unitTypes;
    QMap<SettlementType *, QList<UnitType *>> recruits;
};

} // namespace core
} // namespace warmonger

#endif // CORE_FACTION_H
