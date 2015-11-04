#include <algorithm>
#include <functional>
#include <iterator>

#include <QBrush>
#include <QColor>
#include <QPainter>

#include "core/Game.h"
#include "core/Settlement.h"
#include "core/SettlementType.h"
#include "core/TerrainType.h"
#include "core/Unit.h"
#include "core/UnitType.h"
#include "core/Util.h"
#include "core/WorldSurface.h"
#include "ui/GameMap.h"

static const QString category{"ui"};

using namespace warmonger;
using namespace warmonger::ui;

GameMap::GameMap(QQuickItem *parent) :
    QQuickPaintedItem(parent),
    nodes(),
    world(nullptr),
    surface(nullptr),
    tileSize(),
    game(nullptr),
    nodesInfo(),
    boundingRect(),
    hexagonPainterPath(),
    focusedNode(nullptr),
    currentNodeInfo(nullptr),
    windowPosRect(),
    windowPos(0, 0),
    windowSize(0, 0),
    lastPos(0, 0)
{
    this->setAcceptHoverEvents(true);
    this->setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);

    QObject::connect(
        this,
        &GameMap::widthChanged,
        this,
        &GameMap::onWidthChanged
    );

    QObject::connect(
        this,
        &GameMap::heightChanged,
        this,
        &GameMap::onHeightChanged
    );
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

core::MapNode * GameMap::getCurrentMapNode() const
{
    if (this->currentNodeInfo == nullptr)
        return nullptr;

    return this->currentNodeInfo->node;
}

QObject * GameMap::readCurrentMapNode() const
{
    if (this->currentNodeInfo == nullptr)
        return nullptr;

    return this->currentNodeInfo->node;
}

QPoint GameMap::getCurrentPos() const
{
    if (this->currentNodeInfo != nullptr)
        return this->currentNodeInfo->pos;
    else
        return QPoint();
}

core::Settlement * GameMap::getCurrentSettlement() const
{
    if (this->currentNodeInfo == nullptr)
        return nullptr;

    return this->currentNodeInfo->settlement;
}

QObject * GameMap::readCurrentSettlement() const
{
    if (this->currentNodeInfo == nullptr)
        return nullptr;

    return this->currentNodeInfo->settlement;
}

core::Unit * GameMap::getCurrentUnit() const
{
    if (this->currentNodeInfo == nullptr)
        return nullptr;

    return this->currentNodeInfo->unit;
}

QObject * GameMap::readCurrentUnit() const
{
    if (this->currentNodeInfo == nullptr)
        return nullptr;

    return this->currentNodeInfo->unit;
}

QPoint GameMap::getWindowPos() const
{
    return this->windowPos;
}

void GameMap::setWindowPos(const QPoint& windowPos)
{
    QPoint wp(project(windowPos, this->windowPosRect));

    if (this->windowPos != wp)
    {
        this->windowPos = wp;
        this->update();

        emit windowPosChanged();
    }
}

void GameMap::centerWindow(const QPoint &pos)
{
    QPoint ws(this->windowSize.width(), this->windowSize.height());
    this->setWindowPos(pos - ws / 2);
}

void GameMap::moveWindowBy(const QPoint &diff)
{
    const QPoint newPos(this->windowPos + diff);
    this->setWindowPos(newPos);
}

QSize GameMap::getWindowSize() const
{
    return this->windowSize;
}

void GameMap::paint(QPainter *painter)
{
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing, true);

    const QRect window = QRect(this->windowPos, this->windowSize);
    painter->setWindow(window);

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

    auto cbegin = dirtyNodes.constBegin();
    auto cend = dirtyNodes.constEnd();
    std::function<void(const core::MapNode *)> drawNodeFunc = std::bind(
        &GameMap::drawNode,
        this,
        painter,
        std::placeholders::_1
    );
    std::function<void(const core::MapNode *)> drawGridFunc = std::bind(
        &GameMap::drawGrid,
        this,
        painter,
        std::placeholders::_1
    );
    std::function<void(const core::MapNode *)> drawContentFunc = std::bind(
        &GameMap::drawContent,
        this,
        painter,
        std::placeholders::_1
    );

    std::for_each(cbegin, cend, drawNodeFunc);
    std::for_each(cbegin, cend, drawGridFunc);
    if (this->focusedNode != nullptr)
        this->drawFocusMark(painter, this->focusedNode);
    std::for_each(cbegin, cend, drawContentFunc);

    painter->restore();
}

void GameMap::mousePressEvent(QMouseEvent *event)
{
    this->lastPos = event->pos();
    const QPoint pos(this->mapToMap(event->pos()));

    auto cbegin = this->nodes.constBegin();
    auto cend = this->nodes.constEnd();
    auto it = std::find_if(cbegin, cend, [&](const core::MapNode *node) {
        return this->surface->hexContains(pos - this->nodesInfo[node]->pos);
    });

    core::MapNode *focusedNode = it == cend ? nullptr : *it;
    if (this->focusedNode != focusedNode) {
        this->focusedNode = focusedNode;
        this->update();
    }
}

void GameMap::mouseMoveEvent(QMouseEvent *event)
{
    this->moveWindowBy(lastPos - event->pos());

    this->lastPos = event->pos();
}

void GameMap::hoverMoveEvent(QHoverEvent *event)
{
    const QPoint pos(this->mapToMap(event->pos()));

    auto cbegin = this->nodes.constBegin();
    auto cend = this->nodes.constEnd();
    auto it = std::find_if(cbegin, cend, [&](const core::MapNode *node) {
        return this->surface->hexContains(pos - this->nodesInfo[node]->pos);
    });

    NodeInfo *nodeInfo = it == cend ? nullptr : this->nodesInfo[*it];
    if (this->currentNodeInfo != nodeInfo) {
        this->currentNodeInfo = nodeInfo;
        emit currentMapNodeInfoChanged();
        emit currentMapNodeChanged();
        emit currentPosChanged();
        emit currentSettlementChanged();
        emit currentUnitChanged();
    }
}

void GameMap::setupMap()
{
    // these are used a lot
    this->nodes = this->game->getMapNodes();
    this->world = this->game->getWorld();
    this->surface = this->world->getSurface();
    this->tileSize = this->surface->getTileSize();

    this->hexagonPainterPath = hexagonPath(this->tileSize);

    this->nodesInfo.clear();
    this->nodesInfo.insert(this->nodes[0], new NodeInfo(this->nodes[0]));
    positionNodes(this->nodes[0], this->nodesInfo, this->tileSize);

    const QList<core::Settlement *> settlements = this->game->getSettlements();
    std::for_each(
        settlements.constBegin(),
        settlements.constEnd(),
        [&](core::Settlement *s)
        {
            //FIXME: can settlement's mapnodes be null?
            this->nodesInfo[s->getMapNode()]->settlement = s;
        }
    );

    const QList<core::Unit *> units = this->game->getUnits();
    std::for_each(
        units.constBegin(),
        units.constEnd(),
        [&](core::Unit *u)
        {
            //FIXME: can unit's mapnodes be null?
            this->nodesInfo[u->getMapNode()]->unit = u;
        }
    );

    this->updateGeometry();
}

void GameMap::updateGeometry()
{
    this->boundingRect = calculateBoundingRect(
        this->nodesInfo,
        this->tileSize
    );

    this->updateWindowPosRect();
    this->setWindowPos(this->boundingRect.topLeft());
}

void GameMap::updateWindowPosRect()
{
    this->windowPosRect = QRect(
        this->boundingRect.x(),
        this->boundingRect.y(),
        this->boundingRect.width() - this->windowSize.width(),
        this->boundingRect.height() - this->windowSize.height()
    );
}

QPoint GameMap::mapToMap(const QPoint &p)
{
    return p + this->windowPos;
}

void GameMap::onWidthChanged()
{
    this->windowSize.setWidth(this->width());
    this->updateWindowPosRect();

    emit windowSizeChanged();
}

void GameMap::onHeightChanged()
{
    this->windowSize.setHeight(this->height());
    this->updateWindowPosRect();

    emit windowSizeChanged();
}

bool GameMap::rectContainsNode(const QRect &rect, const core::MapNode *node)
{
    const QPoint pos = this->nodesInfo[node]->pos;
    if (rect.contains(pos))
        return true;

    const QRect nodeRect = QRect(pos, this->tileSize);
    return rect.intersects(nodeRect);
}

void GameMap::drawNode(QPainter *painter, const core::MapNode *node)
{
    const QString terrainTypeName = node->getTerrainType()->objectName();
    QImage image = this->surface->getImage(terrainTypeName);
    QPoint pos = this->nodesInfo[node]->pos;
    painter->drawImage(pos, image);
}

void GameMap::drawGrid(QPainter *painter, const core::MapNode *node)
{
    const QPoint pos = this->nodesInfo[node]->pos;
    const QColor color = this->surface->getColor("grid");
    QPen pen(color);
    pen.setWidth(1);

    painter->save();
    painter->translate(pos);

    painter->strokePath(this->hexagonPainterPath, pen);

    painter->restore();
}

void GameMap::drawFocusMark(QPainter *painter, const core::MapNode *node)
{
    const QPoint pos = this->nodesInfo[node]->pos;
    const QColor color = this->surface->getColor("focusOutline");
    QPen pen(color);
    pen.setWidth(2);

    painter->save();
    painter->translate(pos);

    painter->strokePath(this->hexagonPainterPath, pen);

    painter->restore();
}

void GameMap::drawContent(QPainter *painter, const core::MapNode *node)
{
    const NodeInfo *nodeInfo = this->nodesInfo[node];
    const QPoint pos = nodeInfo->pos;
    const QRect frame(pos, this->tileSize);
    const core::Settlement *settlement = nodeInfo->settlement;
    const core::Unit *unit = nodeInfo->unit;

    if (settlement != nullptr)
    {
        const core::SettlementType *st = settlement->getSettlementType();
        const QImage image = this->surface->getImage(st->objectName());
        painter->drawImage(frame, image);
    }

    if (unit != nullptr)
    {
        const core::UnitType *ut = unit->getUnitType();
        const QImage image = this->surface->getImage(ut->objectName());
        painter->drawImage(frame, image);
    }
}
