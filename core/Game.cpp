#include "core/Game.h"

namespace warmonger {
namespace core {

Game::Game(QObject *parent) :
    QObject(parent),
    campaignMap(nullptr),
    turn(0)
{
}

CampaignMap * Game::getCampaignMap() const
{
    return this->campaignMap;
}

void Game::setCampaignMap(CampaignMap *campaignMap)
{
    if (this->campaignMap != campaignMap)
    {
        this->campaignMap = campaignMap;
        emit campaignMapChanged();
    }
}

int Game::getTurn() const
{
    return this->turn;
}

void Game::setTurn(int turn)
{
    if (this->turn != turn)
    {
        this->turn = turn;
        emit turnChanged();
    }
}

} // namespace core
} // namespace warmonger
