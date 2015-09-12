#include "core/Game.h"

using namespace warmonger::core;

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
    Map::dataFromJson(obj);
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
