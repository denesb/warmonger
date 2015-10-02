#include <QBrush>
#include <QColor>
#include <QPainter>
#include <QRectF>

#include "core/Game.h"
#include "core/Util.h"
#include "core/WorldSurface.h"
#include "ui/GameMap.h"
#include "ui/MapUtil.h"

static const QString category{"ui"};

using namespace warmonger;
using namespace warmonger::ui;

GameMap::GameMap(QQuickItem *parent) :
    QQuickPaintedItem(parent),
    game(nullptr)
{
}

GameMap::~GameMap()
{
}

core::Game * GameMap::getGame() const
{
    return this->game;
}

void GameMap::setGame(core::Game *game)
{
    if (this->game != game)
    {
        this->game = game;
        this->setupMap();
        emit gameChanged();
    }
}

QObject * GameMap::readGame() const
{
    return this->game;
}

void GameMap::writeGame(QObject *game)
{
    core::Game *g = qobject_cast<core::Game *>(game);
    if (g == nullptr)
    {
        wError(category) << "game is null or has wrong type";
        throw core::Exception(core::Exception::InvalidValue);
    }
    this->setGame(g);
}

void GameMap::paint(QPainter *painter)
{
    QBrush brush(QColor("black"));
    painter->fillRect(50, 50, 50, 50, brush);
    QRectF r(0, 0, 50, 50);
    QString text;
    if (this->game != nullptr)
        text = game->getDisplayName();
    painter->drawText(r, text);
}

void GameMap::setupMap()
{
    QList<core::MapNode *> nodes = game->getMapNodes();
    core::WorldSurface *surface = game->getWorld()->getSurface();

    QSet<const core::MapNode *> visitedNodes;
    this->nodePos = positionNodes(
        nodes.first(),
        QPoint(0, 0),
        surface->getTileSize(),
        visitedNodes
    );

    this->updateGeometry();
}

void GameMap::updateGeometry()
{
    QList<core::MapNode *> nodes = game->getMapNodes();
    core::WorldSurface *surface = game->getWorld()->getSurface();

    this->boundingRect = calculateBoundingRect(
        this->game->getMapNodes(),
        this->nodePos,
        surface->getTileSize()
    );
}
