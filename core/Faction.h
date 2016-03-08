#ifndef CORE_FACTION_H
#define CORE_FACTION_H

#include <QObject>
#include <QVariant>

namespace warmonger {
namespace core {

class UnitType;
class SettlementType;

class Faction :
    public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(QVariantList unitTypes READ readUnitTypes NOTIFY unitTypesChanged)
    Q_PROPERTY(QVariantMap recruits READ readRecruits NOTIFY recruitsChanged)

public:
    explicit Faction(QObject *parent=nullptr);
    ~Faction();

    QString getDisplayName() const;
    void setDisplayName(const QString &displayName);

    QList<UnitType *> getUnitTypes() const;
    QVariantList readUnitTypes() const;
    void setUnitTypes(const QList<UnitType *> &unitTypes);
    void addUnitType(UnitType *unitType);

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
    void displayNameChanged();
    void unitTypesChanged();
    void recruitsChanged();

private:
    QString displayName;
    QList<UnitType *> unitTypes;
    QMap<SettlementType *, QList<UnitType *>> recruits;
};

} // namespace core
} // namespace warmonger

#endif // CORE_FACTION_H
