#include <algorithm>
#include <functional>
#include <iterator>

#include <QBrush>
#include <QColor>
#include <QPainter>
#include <QTimer>

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

const qreal GameMap::unitStep{1.0};

GameMap::GameMap(QQuickItem *parent) :
    QQuickPaintedItem(parent),
    game(nullptr),
    world(nullptr),
    surface(nullptr),
    tileSize(),
    nodes(),
    nodesInfo(),
    reachableNodes(),
    pathNodes(),
    focusedNodeInfo(nullptr),
    currentNodeInfo(nullptr),
    movingUnit(nullptr),
    unitPath(),
    nextNodeIndex(0),
    unitPos(),
    unitMoveTimer(new QTimer()),
    boundingRect(),
    hexagonPainterPath(),
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

    QObject::connect(
        this->unitMoveTimer,
        &QTimer::timeout,
        this,
        &GameMap::advanceUnit
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
    std::function<void(core::MapNode *)> drawOverlayFunc = std::bind(
        &GameMap::drawOverlay,
        this,
        painter,
        std::placeholders::_1
    );

    std::for_each(cbegin, cend, drawNodeFunc);
    std::for_each(cbegin, cend, drawGridFunc);
    if (this->focusedNodeInfo != nullptr)
        this->drawFocusMark(painter, this->focusedNodeInfo->node);
    std::for_each(cbegin, cend, drawContentFunc);
    std::for_each(cbegin, cend, drawOverlayFunc);

    if (this->movingUnit != nullptr)
    {
        this->drawMovingUnit(painter);
    }

    painter->restore();
}

void GameMap::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
    {
        this->updateFocus(event->pos());
    }
    else if (event->buttons() == Qt::RightButton)
    {
        this->moveUnit(this->mapToMap(event->pos()));
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
    NodeInfo *nodeInfo = findNodeInfo(this->surface, this->nodesInfo, pos);

    if (this->currentNodeInfo != nodeInfo) {
        this->currentNodeInfo = nodeInfo;
        this->onCurrentNodeInfoChanged();
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

void GameMap::onFocusedNodeInfoChanged()
{
    if (this->focusedNodeInfo != nullptr &&
        this->focusedNodeInfo->unit != nullptr)
    {
        this->reachableNodes = this->game->reachableMapNodes(
            this->focusedNodeInfo->unit
        );
    }
    else
    {
        this->reachableNodes.clear();
    }

    emit focusedMapNodeInfoChanged();
    emit focusedMapNodeChanged();
    emit focusedPosChanged();
    emit focusedSettlementChanged();
    emit focusedUnitChanged();

    this->update();
}

void GameMap::onCurrentNodeInfoChanged()
{
    if (this->focusedNodeInfo != nullptr &&
        this->focusedNodeInfo->unit != nullptr &&
        this->currentNodeInfo != nullptr)
    {
        QList<core::MapNode *> path = this->game->shortestPath(
            this->focusedNodeInfo->unit,
            this->focusedNodeInfo->node,
            this->currentNodeInfo->node
        );
        this->pathNodes = QSet<core::MapNode *>::fromList(path);
    }
    else
    {
        this->pathNodes.clear();
    }

    emit currentMapNodeInfoChanged();
    emit currentMapNodeChanged();
    emit currentPosChanged();
    emit currentSettlementChanged();
    emit currentUnitChanged();

    this->update();
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

    if (unit != nullptr && unit != this->movingUnit)
    {
        const core::UnitType *ut = unit->getUnitType();
        const QImage image = this->surface->getImage(ut->objectName());
        painter->drawImage(frame, image);
    }
}

void GameMap::drawMovingUnit(QPainter *painter)
{
    const QPoint pos(this->unitPos.toPoint());
    const QRect frame(pos, this->tileSize);
    const core::UnitType *ut = this->movingUnit->getUnitType();
    const QImage image = this->surface->getImage(ut->objectName());
    painter->drawImage(frame, image);
}

void GameMap::drawOverlay(QPainter *painter, core::MapNode *node)
{
    if (this->focusedNodeInfo == nullptr ||
        this->focusedNodeInfo->unit == nullptr)
        return;

    bool hasOverlay = false;
    QColor color;
    if (this->reachableNodes.contains(node))
    {
        if (this->pathNodes.contains(node))
        {
            hasOverlay = true;
            color.setNamedColor("green");
        }
    }
    else
    {
        hasOverlay = true;
        color.setNamedColor("black");
    }

    if (hasOverlay)
    {
        color.setAlphaF(0.5);
        const QBrush brush(color);

        painter->save();
        painter->translate(this->nodesInfo[node]->pos);

        painter->fillPath(this->hexagonPainterPath, brush);

        painter->restore();
    }
}

void GameMap::updateFocus(const QPoint &p)
{
    this->lastPos = p;
    const QPoint pos(this->mapToMap(p));
    NodeInfo *focusedNodeInfo = findNodeInfo(
        this->surface,
        this->nodesInfo,
        pos
    );

    if (this->focusedNodeInfo != focusedNodeInfo) {
        this->focusedNodeInfo = focusedNodeInfo;
        this->onFocusedNodeInfoChanged();
    }
}

void GameMap::moveUnit(const QPoint &p)
{
    if (this->focusedNodeInfo == nullptr ||
        this->focusedNodeInfo->unit == nullptr)
        return;

    NodeInfo *destinationNodeInfo = findNodeInfo(
        this->surface,
        this->nodesInfo,
        p
    );

    if (destinationNodeInfo == nullptr ||
        destinationNodeInfo == this->focusedNodeInfo)
        return;

    this->movingUnit = this->focusedNodeInfo->unit;
    this->unitPath = this->game->shortestPath(
        this->focusedNodeInfo->unit,
        this->focusedNodeInfo->node,
        destinationNodeInfo->node
    );
    this->nextNodeIndex = 1;
    this->unitPos = this->focusedNodeInfo->pos;

    this->unitMoveTimer->start(1000/20);
}

void GameMap::advanceUnit()
{
    if (this->movingUnit == nullptr)
        return;

    core::MapNode *nextNode = this->unitPath[this->nextNodeIndex];
    NodeInfo *nextNodeInfo = this->nodesInfo[nextNode];

    qreal distTravelled = this->stepUnitTorwards(nextNodeInfo);

    if (distTravelled != 0 && this->nextNodeIndex != this->unitPath.size())
    {
        this->nextNodeIndex++;
        nextNode = this->unitPath[this->nextNodeIndex];
        nextNodeInfo = this->nodesInfo[nextNode];

        this->stepUnitTorwards(nextNodeInfo);
    }

    core::MapNode *destinationNode = this->unitPath.last();
    NodeInfo *destinationNodeInfo = this->nodesInfo[destinationNode];
    if (this->unitPos == destinationNodeInfo->pos)
    {
        this->unitMoveTimer->stop();
        core::MapNode *originalNode = this->movingUnit->getMapNode();
        NodeInfo *originalNodeInfo = this->nodesInfo[originalNode];

        this->movingUnit->setMapNode(destinationNode);
        originalNodeInfo->unit = nullptr;
        destinationNodeInfo->unit = this->movingUnit;

        this->movingUnit = nullptr;
    }

    this->update();
}

qreal GameMap::stepUnitTorwards(NodeInfo *n)
{
    QLineF path(this->unitPos, n->pos);

    if (path.length() > GameMap::unitStep)
        path.setLength(GameMap::unitStep);

    this->unitPos = n->pos;

    return GameMap::unitStep - path.length();
}
