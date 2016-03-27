#ifndef CORE_PLAYER_H
#define CORE_PLAYER_H

#include <QColor>
#include <QObject>
#include <QString>

#include "core/Faction.h"

namespace warmonger {
namespace core {

class Player :
    public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(QColor color READ getColor WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(int goldBalance READ getGoldBalance WRITE setGoldBalance NOTIFY goldBalanceChanged)
    Q_PROPERTY(Faction *faction READ getFaction WRITE setFaction NOTIFY factionChanged)

public:
    explicit Player(QObject *parent=nullptr);
    ~Player();

    QString getDisplayName() const;
    void setDisplayName(const QString &displayName);

    QColor getColor() const;
    void setColor(const QColor &color);

    int getGoldBalance() const;
    void setGoldBalance(int goldBalance);

    Faction * getFaction() const;
    void setFaction(Faction *faction);

signals:
    void displayNameChanged();
    void colorChanged();
    void goldBalanceChanged();
    void factionChanged();

private:
    QString displayName;
    QColor color;
    int goldBalance;
    Faction *faction;
};

} // namespace core
} // namespace warmonger

#endif // CORE_PLAYER_H
