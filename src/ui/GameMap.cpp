#include <algorithm>
#include <functional>
#include <iterator>

#include <QBrush>
#include <QColor>
#include <QPainter>
#include <QRectF>

#include "core/Game.h"
#include "core/TerrainType.h"
#include "core/Util.h"
#include "core/WorldSurface.h"
#include "ui/GameMap.h"
#include "ui/MapUtil.h"

static const QString category{"ui"};

using namespace warmonger;
using namespace warmonger::ui;

GameMap::GameMap(QQuickItem *parent) :
    QQuickPaintedItem(parent),
    game(nullptr),
    nodePos(),
    boundingRect(),
    nodes(),
    world(nullptr),
    surface(nullptr),
    tileSize()
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
    const QRect window = painter->window();
    std::function<bool(const core::MapNode *)> filterFunc = std::bind(
        &GameMap::rectContainsNode,
        this,
        window,
        std::placeholders::_1
    );
    QList<core::MapNode *> dirtyNodes;
    std::copy_if(
        this->nodes.constBegin(),
        this->nodes.constEnd(),
        std::back_inserter(dirtyNodes),
        filterFunc
    );

    wDebug(category) << window;
    wDebug(category) << dirtyNodes.size();

    auto cbegin = dirtyNodes.constBegin();
    auto cend = dirtyNodes.constEnd();

    std::function<void(const core::MapNode *)> drawNodeFunc = std::bind(
        &GameMap::drawNode,
        this,
        painter,
        std::placeholders::_1
    );
    std::for_each(cbegin, cend, drawNodeFunc);

    std::function<void(const core::MapNode *)> drawGridFunc = std::bind(
        &GameMap::drawGrid,
        this,
        painter,
        std::placeholders::_1
    );
    std::for_each(cbegin, cend, drawGridFunc);
}

void GameMap::setupMap()
{
    // these are used a lot
    this->nodes = this->game->getMapNodes();
    this->world = this->game->getWorld();
    this->surface = this->world->getSurface();
    this->tileSize = this->surface->getTileSize();

    QSet<const core::MapNode *> visitedNodes;
    this->nodePos = positionNodes(
        this->nodes[0],
        QPoint(0, 0),
        this->tileSize,
        visitedNodes
    );

    this->updateGeometry();
}

void GameMap::updateGeometry()
{
    this->boundingRect = calculateBoundingRect(
        this->nodes,
        this->nodePos,
        this->tileSize
    );
}

bool GameMap::rectContainsNode(const QRect &rect, const core::MapNode *node)
{
    const QPoint pos = this->nodePos[node];
    if (!rect.contains(pos))
        return false;

    const QRect nodeRect = QRect(pos, this->tileSize);
    return rect.intersects(nodeRect);
}

void GameMap::drawNode(QPainter *painter, const core::MapNode *node)
{
    const QString terrainTypeName = node->getTerrainType()->objectName();
    QImage image = this->surface->getImage(terrainTypeName);
    QPoint pos = this->nodePos[node];
    painter->drawImage(pos, image);
}

void GameMap::drawGrid(QPainter *painter, const core::MapNode *node)
{
    QPainterPath path = hexagonPath(this->tileSize);
    const QPoint pos = this->nodePos[node];
    const QColor color = this->surface->getColor("grid");
    QPen pen(color);
    pen.setWidth(2);

    painter->save();
    painter->translate(pos);

    painter->strokePath(path, pen);

    painter->restore();
}
