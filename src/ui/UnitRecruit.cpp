#include <algorithm>
#include <functional>
#include <iterator>

#include <QPainter>

#include "core/Util.h"
#include "core/WorldSurface.h"
#include "ui/MapUtil.h"
#include "ui/UnitRecruit.h"

static const QString category{"ui"};

using namespace warmonger;
using namespace warmonger::ui;

UnitRecruit::UnitRecruit(QQuickItem *parent) :
    QQuickPaintedItem(parent),
    game(nullptr),
    world(nullptr),
    surface(nullptr),
    tileSize(),
    mapDrawer(nullptr),
    nodes(),
    nodesPos(),
    focusedNode(nullptr),
    currentNode(nullptr),
    boundingRect(),
    lastPos(0, 0)
{
    this->setAcceptedMouseButtons(Qt::LeftButton);

    QObject::connect(
        this,
        &UnitRecruit::widthChanged,
        this,
        &UnitRecruit::updateTransform
    );

    QObject::connect(
        this,
        &UnitRecruit::heightChanged,
        this,
        &UnitRecruit::updateTransform
    );
}

UnitRecruit::~UnitRecruit()
{
}

core::Game * UnitRecruit::getGame() const
{
    return this->game;
}

void UnitRecruit::setGame(core::Game *game)
{
    if (this->game != game)
    {
        this->game = game;
        this->world = this->game->getWorld();
        this->surface = this->world->getSurface();
        this->tileSize = this->surface->getTileSize();

        emit gameChanged();
    }
}

core::Settlement * UnitRecruit::getSettlement() const
{
    return this->settlement;
}

void UnitRecruit::setSettlement(core::Settlement *settlement)
{
    if (this->settlement != settlement)
    {
        this->settlement = settlement;
        this->setupMap();

        emit settlementChanged();
    }
}

void UnitRecruit::paint(QPainter *painter)
{
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->scale(this->scale, this->scale);
    painter->translate(this->translate);

    DrawingInfo drawingInfo;
    drawingInfo.focusedNode = this->focusedNode;

    this->mapDrawer->drawMap(painter, this->nodes, drawingInfo);

    painter->restore();
}

void UnitRecruit::mousePressEvent(QMouseEvent *event)
{
    this->updateFocus(event->pos());
}

void UnitRecruit::setupMap()
{
    QList<core::MapNode *> nodes;

    core::MapNode *node = settlement->getMapNode();
    nodes << node;
    nodes << node->getNeighbours().values();

    this->nodes = nodes;
    QHash<const core::MapNode *, QPoint> allNodesPos = positionNodes(
        node,
        this->tileSize
    );

    this->nodesPos.clear();
    for (core::MapNode *node : this->nodes)
    {
        this->nodesPos[node] = allNodesPos[node];
    }

    if (this->mapDrawer)
        delete this->mapDrawer;

    this->mapDrawer = new MapDrawer(this->game, this->nodesPos);

    this->updateGeometry();
}

void UnitRecruit::updateGeometry()
{
    this->boundingRect = calculateBoundingRect(
        this->nodesPos,
        this->tileSize
    );

    this->updateTransform();
}

void UnitRecruit::updateTransform()
{
    QPair<qreal, QPointF> transform = centerIn(
        this->boundingRect,
        this->contentsBoundingRect()
    );
    this->scale = transform.first;
    this->translate = transform.second;
}

QPointF UnitRecruit::mapToMap(const QPointF &p) const
{
    const qreal rscale = 1 / this->scale;
    return p * rscale - this->translate;
}

bool UnitRecruit::rectContainsNode(const QRect &rect, const core::MapNode *node)
{
    const QPoint pos = this->nodesPos[node];
    if (rect.contains(pos))
        return true;

    const QRect nodeRect = QRect(pos, this->tileSize);
    return rect.intersects(nodeRect);
}

void UnitRecruit::updateFocus(const QPoint &p)
{
    this->lastPos = p;
    const QPointF posf(this->mapToMap(p));
    const QPoint pos = posf.toPoint();
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
    }
}
