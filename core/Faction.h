#ifndef CORE_FACTION_H
#define CORE_FACTION_H

#include <QColor>
#include <QObject>
#include <QString>

#include "core/Civilization.h"

namespace warmonger {
namespace core {

class Faction : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(QColor color READ getColor WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(int goldBalance READ getGoldBalance WRITE setGoldBalance NOTIFY goldBalanceChanged)
    Q_PROPERTY(Civilization* civilization READ getCivilization WRITE setCivilization NOTIFY civilizationChanged)

public:
    explicit Faction(QObject* parent = nullptr);

    QString getDisplayName() const;
    void setDisplayName(const QString& displayName);

    QColor getColor() const;
    void setColor(const QColor& color);

    int getGoldBalance() const;
    void setGoldBalance(int goldBalance);

    Civilization* getCivilization() const;
    void setCivilization(Civilization* civilization);

signals:
    void displayNameChanged();
    void colorChanged();
    void goldBalanceChanged();
    void civilizationChanged();

private:
    QString displayName;
    QColor color;
    int goldBalance;
    Civilization* civilization;
};

} // namespace core
} // namespace warmonger

#endif // CORE_FACTION_H
