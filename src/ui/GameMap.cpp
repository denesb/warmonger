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

namespace warmonger {
namespace ui {

struct MovingUnit
{
    MovingUnit(
        core::Unit *unit,
        const QList<core::MapNode *> &path,
        const QPointF &pos
    ) :
        unit(unit),
        path(path),
        pos(pos),
        index(0)
    {
        MovingUnit::movingUnits.insert(unit);
    }

    ~MovingUnit()
    {
        MovingUnit::movingUnits.remove(unit);
    }

    core::MapNode * nextNode() const
    {
        return this->path[this->index];
    }

    static const qreal unitStep;
    static QSet<const core::Unit *> movingUnits;

    core::Unit *unit;
    QList<core::MapNode *> path;
    QPointF pos;
    int index;
};

QSet<const core::Unit *> MovingUnit::movingUnits{};
const qreal MovingUnit::unitStep{10.0};

} // namespace ui
} // namespace warmonger

using namespace warmonger;
using namespace warmonger::ui;

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
    unitMoveTimer(new QTimer()),
    movingUnits(),
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
        &GameMap::advanceUnits
    );

    this->unitMoveTimer->start(1000/25);
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

    std::function<void(MovingUnit *)> drawMovingUnitFunc = std::bind(
        &GameMap::drawMovingUnit,
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

    std::for_each(
        this->movingUnits.constBegin(),
        this->movingUnits.constEnd(),
        drawMovingUnitFunc
    );

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

    if (unit != nullptr && !MovingUnit::movingUnits.contains(unit))
    {
        const core::UnitType *ut = unit->getUnitType();
        const QImage image = this->surface->getImage(ut->objectName());
        painter->drawImage(frame, image);
    }
}

void GameMap::drawMovingUnit(QPainter *painter, const MovingUnit *movingUnit)
{
    const QPoint pos(movingUnit->pos.toPoint());
    const QRect frame(pos, this->tileSize);
    const core::UnitType *ut = movingUnit->unit->getUnitType();
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

    NodeInfo *destNodeInfo = findNodeInfo(
        this->surface,
        this->nodesInfo,
        p
    );

    if (destNodeInfo == nullptr || destNodeInfo == this->focusedNodeInfo)
        return;

    QList<core::MapNode *> path = this->game->shortestPath(
        this->focusedNodeInfo->unit,
        this->focusedNodeInfo->node,
        destNodeInfo->node
    );

    core::Unit *unit = this->focusedNodeInfo->unit;
    unit->setMapNode(destNodeInfo->node);

    this->focusedNodeInfo->unit = nullptr;
    destNodeInfo->unit = unit;

    MovingUnit *movingUnit = new MovingUnit(
        unit,
        path,
        this->focusedNodeInfo->pos
    );
    this->movingUnits.append(movingUnit);
}

void GameMap::advanceUnits()
{
    if (this->movingUnits.empty())
        return;

    for (MovingUnit *movingUnit : this->movingUnits)
    {
        NodeInfo *nextNode = this->nodesInfo[movingUnit->nextNode()];
        qreal distTravelled = this->stepUnitTorwards(movingUnit, nextNode);

        if (distTravelled != 0 &&
            movingUnit->index < movingUnit->path.size() - 1)
        {
            movingUnit->index++;
            nextNode = this->nodesInfo[movingUnit->nextNode()];

            this->stepUnitTorwards(movingUnit, nextNode);
        }

        core::MapNode *destNode = movingUnit->unit->getMapNode();
        NodeInfo *destNodeInfo = this->nodesInfo[destNode];
        if (movingUnit->pos == destNodeInfo->pos)
        {
            this->movingUnits.removeOne(movingUnit);
            delete movingUnit;
        }
    }

    this->update();
}

qreal GameMap::stepUnitTorwards(MovingUnit *u, NodeInfo *n)
{
    QLineF path(u->pos, n->pos);

    if (path.length() > MovingUnit::unitStep)
        path.setLength(MovingUnit::unitStep);

    u->pos = path.p2();

    return MovingUnit::unitStep - path.length();
}
