#include "core/Game.h"

using namespace warmonger::core;

static const QString category{"core"};

Game::Game() :
    Map(),
    turnNumber(0),
    playerIndex(0)
{
}

Game::~Game()
{
}

QString Game::specification(const QString &objectName) const
{
    return "games:" + objectName + ".wgd";
}

void Game::fromMapJson(const QJsonObject &obj)
{
    Map::fromJson(obj);
    this->turnNumber = 0;
    this->playerIndex = 0;
}

void Game::dataFromJson(const QJsonObject &obj)
{
    Map::dataFromJson(obj);
    this->turnNumber = obj["turnNumber"].toInt();
    this->playerIndex = obj["playerIndex"].toInt();
}

void Game::dataToJson(QJsonObject &obj) const
{
    Map::dataToJson(obj);
}
