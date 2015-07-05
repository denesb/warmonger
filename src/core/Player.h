#ifndef CORE_PLAYER_H
#define CORE_PLAYER_H

#include <QString>
#include <QColor>
#include <QList>

#include "core/GameObject.h"

namespace warmonger {
namespace core {

class Faction;
class Unit;
class Settlement;

class Player :
    public GameObject
{
    Q_OBJECT

public:
    Player(QObject *parent = nullptr);
    ~Player();

    QString getDisplayName() const;
    void setDisplayName(const QString &displayName);

    QColor getColor() const;
    void setColor(const QColor &color);

    int getGoldBalance() const;
    void setGoldBalance(int goldBalance);

    Faction * getFaction() const;
    void setFaction(Faction *faction);

    QList<Unit *> getUnits() const;
    void setUnits(const QList<Unit *> &units);

    QList<Settlement *> getSettlemets() const;
    void setSettlements(const QList<Settlement *> &settlements);

    void fromJson(const QJsonObject &obj);
    QJsonObject toJson() const;

private:
    QString displayName;
    QColor color;
    int goldBalance;
    Faction *faction;
    QList<Unit *> units;
    QList<Settlement *> settlements;
};

}; // namespace core
}; // namespace warmonger

#endif // CORE_PLAYER_H
