#ifndef CORE_CIVILIZATION_H
#define CORE_CIVILIZATION_H

#include <map>
#include <vector>

#include <QObject>
#include <QVariant>

namespace warmonger {
namespace core {

class UnitType;
class SettlementType;

class Civilization :
    public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(QVariantList unitTypes READ readUnitTypes NOTIFY unitTypesChanged)
    Q_PROPERTY(QVariantMap recruits READ readRecruits NOTIFY recruitsChanged)

public:
    explicit Civilization(QObject *parent=nullptr);

    QString getDisplayName() const;
    void setDisplayName(const QString &displayName);

    std::vector<UnitType *> getUnitTypes() const;
    QVariantList readUnitTypes() const;
    void setUnitTypes(const std::vector<UnitType *> &unitTypes);
    void addUnitType(UnitType *unitType);

    std::map<SettlementType *, std::vector<UnitType *>> getRecruits() const;
    QVariantMap readRecruits() const;
    void setRecruits(const std::map<SettlementType *, std::vector<UnitType *>> &recruits);

    std::vector<UnitType *> getRecruitsFor(SettlementType *settlementType) const;
    bool canRecruitFrom(SettlementType *settlemntType, UnitType *unitType) const;

signals:
    void displayNameChanged();
    void unitTypesChanged();
    void recruitsChanged();

private:
    QString displayName;
    std::vector<UnitType *> unitTypes;
    std::map<SettlementType *, std::vector<UnitType *>> recruits;
};

} // namespace core
} // namespace warmonger

#endif // CORE_CIVILIZATION_H
