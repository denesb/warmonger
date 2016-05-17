#ifndef CORE_SETTLEMENT_TYPE_H
#define CORE_SETTLEMENT_TYPE_H

#include <vector>

#include <QObject>
#include <QVariant>

namespace warmonger {
namespace core {

class UnitType;

class SettlementType :
    public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(int goldPerTurn READ getGoldPerTurn WRITE setGoldPerTurn NOTIFY goldPerTurnChanged)
    Q_PROPERTY(QVariantList recruits READ readRecruits WRITE writeRecruits NOTIFY recruitsChanged)

public:
    explicit SettlementType(QObject *parent=nullptr);
    ~SettlementType();

    QString getDisplayName() const;
    void setDisplayName(const QString &displayName);

    int getGoldPerTurn() const;
    void setGoldPerTurn(int goldPerTurn);

    std::vector<UnitType *> getRecruits() const;
    void setRecruits(const std::vector<UnitType *> &recruits);
    QVariantList readRecruits() const;
    void writeRecruits(QVariantList recruits);

signals:
    void displayNameChanged();
    void goldPerTurnChanged();
    void recruitsChanged();

private:
    QString displayName;
    int goldPerTurn;
    std::vector<UnitType *> recruits;
};

} // namespace core
} // namespace warmonger

#endif // CORE_SETTLEMENT_TYPE_H
