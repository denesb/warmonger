#ifndef CORE_PLAYER_H
#define CORE_PLAYER_H

#include <QString>
#include <QColor>
#include <QList>

#include "core/GameObject.h"

namespace warmonger {
namespace core {

class Faction;

class Player :
    public GameObject
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ getColor WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(int goldBalance READ getGoldBalance WRITE setGoldBalance NOTIFY goldBalanceChanged)

public:
    Player(QObject *parent);
    ~Player();

    QColor getColor() const;
    void setColor(const QColor &color);

    int getGoldBalance() const;
    void setGoldBalance(int goldBalance);

    const Faction * getFaction() const;
    void setFaction(const Faction *faction);

signals:
    void colorChanged();
    void goldBalanceChanged();

private:
    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;

    QColor color;
    int goldBalance;
    const Faction *faction;
};

} // namespace core
} // namespace warmonger

#endif // CORE_PLAYER_H
