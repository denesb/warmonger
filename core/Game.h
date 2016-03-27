#ifndef CORE_GAME_H
#define CORE_GAME_H

#include <QObject>

#include "core/CampaignMap.h"

namespace warmonger {
namespace core {

class Game :
    public QObject
{
    Q_OBJECT

    Q_PROPERTY(int turn READ getTurn WRITE setTurn NOTIFY turnChanged)

public:
    explicit Game(QObject *parent=nullptr);

    CampaignMap * getCampaignMap() const;
    void setCampaignMap(CampaignMap *campaignMap);

    int getTurn() const;
    void setTurn(int turn);

signals:
    void campaignMapChanged();
    void turnChanged();

private:
    CampaignMap *campaignMap;
    int turn;
};

} // namespace core
} // namespace warmonger

#endif // CORE_GAME_H
