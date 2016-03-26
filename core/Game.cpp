#include <algorithm>
#include <functional>
#include <limits>
#include <typeinfo>

#include "core/Exception.h"
#include "core/Game.h"
#include "core/MapNode.h"
#include "core/UnitClass.h"
#include "core/UnitType.h"
#include "core/Unit.h"

using namespace warmonger::core;

static const QString loggerName{"core.Game"};

Game::Game(QObject *parent) :
    QObject(parent),
    map(nullptr),
    turn(0)
{
}

Map * Game::getMap() const
{
    return this->map;
}

void Game::setMap(Map *map)
{
    if (this->map != map)
    {
        this->map = map;
        emit mapChanged();
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
