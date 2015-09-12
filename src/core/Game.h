#ifndef CORE_GAME_H
#define CORE_GAME_H

#include "core/Map.h"

namespace warmonger {
namespace core {

class Game :
    public Map
{
    Q_OBJECT

public:

    Game();
    ~Game();

    virtual QString specification(const QString &objectName) const;

    void fromMapJson(const QJsonObject &obj);

signals:

private:
    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;

    int turnNumber;
    int playerIndex;
};

} // namespace core
} // namespace warmonger

#endif // CORE_GAME_H
