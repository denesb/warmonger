#include <algorithm>
#include <functional>
#include <iterator>

#include <QPainter>
#include <QTimer>

#include "core/SettlementType.h"
#include "core/TerrainType.h"
#include "core/UnitType.h"
#include "core/Util.h"
#include "core/WorldSurface.h"
#include "ui/GameMap.h"
#include "ui/MapDrawer.h"
#include "ui/MapUtil.h"

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
    }

    ~MovingUnit()
    {
    }

    core::MapNode * nextNode() const
    {
        return this->path[this->index];
    }

    static const double unitStep;
    static QSet<const core::Unit *> movingUnits;

    core::Unit *unit;
    QList<core::MapNode *> path;
    QPointF pos;
    int index;
};

const double MovingUnit::unitStep{10.0};

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
    mapDrawer(nullptr),
    nodes(),
    nodesPos(),
    reachableNodes(),
    pathNodes(),
    focusedNode(nullptr),
    currentNode(nullptr),
    unitMoveTimer(new QTimer()),
    movingUnits(),
    boundingRect(),
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

    //FIXME: get this magic number from config?
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

core::MapNode * GameMap::getFocusedMapNode() const
{
    return this->focusedNode;
}

core::Settlement * GameMap::getFocusedSettlement() const
{
    if (this->focusedNode == nullptr)
        return nullptr;

    return this->game->getSettlementOn(this->focusedNode);
}

core::Unit * GameMap::getFocusedUnit() const
{
    if (this->focusedNode == nullptr)
        return nullptr;

    return this->game->getUnitOn(this->focusedNode);
}

core::MapNode * GameMap::getCurrentMapNode() const
{
    return this->currentNode;
}

core::Settlement * GameMap::getCurrentSettlement() const
{
    if (this->currentNode == nullptr)
        return nullptr;

    return this->game->getSettlementOn(this->currentNode);
}

core::Unit * GameMap::getCurrentUnit() const
{
    if (this->currentNode == nullptr)
        return nullptr;

    return this->game->getUnitOn(this->currentNode);
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

GameMap::Mode GameMap::getMode() const
{
    return this->mode;
}

void GameMap::setMode(GameMap::Mode mode)
{
    if (this->mode != mode)
    {
        this->mode = mode;
        emit modeChanged();
    }
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

    DrawingInfo drawingInfo;
    drawingInfo.focusedNode = this->focusedNode;

    DrawingInfo::Overlay reachableNodesOverlay;
    reachableNodesOverlay.color = QColor("white");
    reachableNodesOverlay.color.setAlphaF(0.2);
    reachableNodesOverlay.nodes = this->reachableNodes;

    DrawingInfo::Overlay pathNodesOverlay;
    pathNodesOverlay.color = QColor("green");
    pathNodesOverlay.color.setAlphaF(0.4);
    pathNodesOverlay.nodes = this->pathNodes;

    drawingInfo.overlays << reachableNodesOverlay;
    drawingInfo.overlays << pathNodesOverlay;

    for (MovingUnit *movingUnit : this->movingUnits)
    {
        drawingInfo.movingUnits[movingUnit->unit] = movingUnit->pos.toPoint();
    }

    this->mapDrawer->drawMap(painter, dirtyNodes, drawingInfo);

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
    auto it = std::find_if(
        this->nodes.constBegin(),
        this->nodes.constEnd(),
        [&](const core::MapNode *n)
        {
            return surface->hexContains(pos - this->nodesPos[n]);
        }
    );
    core::MapNode *node = it == this->nodes.constEnd() ? nullptr : *it;

    if (this->currentNode != node) {
        this->currentNode = node;
        this->onCurrentNodeChanged();
    }
}

void GameMap::setupMap()
{
    // these are used a lot
    this->nodes = this->game->getMapNodes();
    this->world = this->game->getWorld();
    this->surface = this->world->getSurface();
    this->tileSize = this->surface->getTileSize();

    this->nodesPos = positionNodes(this->nodes, this->tileSize);

    if (this->mapDrawer)
        delete this->mapDrawer;

    this->mapDrawer = new MapDrawer(this->game, this->nodesPos);

    this->updateGeometry();
}

void GameMap::updateGeometry()
{
    this->boundingRect = calculateBoundingRect(
        this->nodesPos,
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

void GameMap::onFocusedNodeChanged()
{
    this->reachableNodes.clear();
    this->pathNodes.clear();

    if (this->focusedNode != nullptr && this->game->hasUnit(this->focusedNode))
    {
        this->reachableNodes = this->game->reachableMapNodes(
            this->game->getUnitOn(this->focusedNode)
        );
    }

    emit focusedMapNodeChanged();
    emit focusedSettlementChanged();
    emit focusedUnitChanged();

    this->update();
}

void GameMap::onCurrentNodeChanged()
{
    if (this->focusedNode != nullptr &&
        this->game->hasUnit(this->focusedNode) &&
        this->currentNode != nullptr)
    {
        QList<core::MapNode *> path = this->game->shortestPath(
            this->game->getUnitOn(this->focusedNode),
            this->focusedNode,
            this->currentNode
        );
        this->pathNodes = QSet<core::MapNode *>::fromList(path);
    }
    else
    {
        this->pathNodes.clear();
    }

    emit currentMapNodeChanged();
    emit currentSettlementChanged();
    emit currentUnitChanged();

    this->update();
}

bool GameMap::rectContainsNode(const QRect &rect, const core::MapNode *node)
{
    const QPoint pos = this->nodesPos[node];
    if (rect.contains(pos))
        return true;

    const QRect nodeRect = QRect(pos, this->tileSize);
    return rect.intersects(nodeRect);
}

void GameMap::updateFocus(const QPoint &p)
{
    this->lastPos = p;
    const QPoint pos(this->mapToMap(p));
    auto it = std::find_if(
        this->nodes.constBegin(),
        this->nodes.constEnd(),
        [&](const core::MapNode *n)
        {
            return surface->hexContains(pos - this->nodesPos[n]);
        }
    );
    core::MapNode *focusedNode = it == this->nodes.constEnd() ? nullptr : *it;

    if (this->focusedNode != focusedNode) {
        this->focusedNode = focusedNode;
        this->onFocusedNodeChanged();
    }
}

void GameMap::moveUnit(const QPoint &p)
{
    core::Unit * unit = this->game->getUnitOn(this->focusedNode);
    if (this->focusedNode == nullptr || unit == nullptr)
        return;

    auto it = std::find_if(
        this->nodes.constBegin(),
        this->nodes.constEnd(),
        [&](const core::MapNode *n)
        {
            return surface->hexContains(p - this->nodesPos[n]);
        }
    );
    core::MapNode *destNode = it == this->nodes.constEnd() ? nullptr : *it;

    if (destNode == nullptr || destNode == this->focusedNode)
        return;

    QList<core::MapNode *> path = this->game->moveUnitToNode(
        unit,
        destNode
    );

    MovingUnit *movingUnit = new MovingUnit(
        unit,
        path,
        this->nodesPos[this->focusedNode]
    );
    this->movingUnits.append(movingUnit);
}

void GameMap::advanceUnits()
{
    if (this->movingUnits.empty())
        return;

    for (MovingUnit *movingUnit : this->movingUnits)
    {
        double l = MovingUnit::unitStep;

        while (l > 0 && movingUnit->index < movingUnit->path.size())
        {
            core::MapNode *nextNode = movingUnit->nextNode();
            QLineF path(movingUnit->pos, this->nodesPos[nextNode]);

            if (path.length() > MovingUnit::unitStep)
                path.setLength(MovingUnit::unitStep);
            else
                movingUnit->index++;

            l -= path.length();
            movingUnit->pos = path.p2();
        }

        if (movingUnit->index == movingUnit->path.size())
        {
            this->movingUnits.removeOne(movingUnit);
            delete movingUnit;
        }
    }

    this->update();
}
