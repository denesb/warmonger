#include <algorithm>
#include <functional>
#include <iterator>

#include <QPainter>

#include "core/Exception.h"
#include "core/WorldSurface.h"
#include "ui/MapUtil.h"
#include "ui/UnitRecruit.h"

static const QString loggerName{"ui.UnitRecruit"};

using namespace warmonger;
using namespace warmonger::ui;

UnitRecruit::UnitRecruit(QQuickItem *parent) :
    QQuickPaintedItem(parent),
    game(nullptr),
    world(nullptr),
    surface(nullptr),
    tileSize(),
    settlement(nullptr),
    unitType(nullptr),
    mapDrawer(nullptr),
    nodes(),
    nodesPos(),
    focusedNode(nullptr),
    boundingRect(),
    error(),
    canRecruit(false)
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
    if (this->mapDrawer)
        delete this->mapDrawer;
}

core::Game * UnitRecruit::getGame() const
{
    return this->game;
}

void UnitRecruit::setGame(core::Game *game)
{
    if (this->game != game)
    {
        if (this->game != nullptr)
            QObject::disconnect(
                this->game,
                nullptr,
                this,
                nullptr
            );

        wInfo(loggerName) << "Game changed " << this->game
            << " -> " << game;

        this->game = game;
        this->world = this->game->getWorld();
        this->surface = this->world->getSurface();
        this->tileSize = this->surface->getTileSize();

        QObject::connect(
            this->game,
            &core::Game::unitAdded,
            this,
            &UnitRecruit::onUnitAdded
        );

        this->updateCanRecruit();

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
        wInfo(loggerName) << "Settlement changed " << this->settlement
            << " -> " << settlement;

        this->settlement = settlement;

        this->setupMap();
        this->selectFocusNode();

        this->updateCanRecruit();

        emit settlementChanged();
    }
}

core::UnitType * UnitRecruit::getUnitType() const
{
    return this->unitType;
}

void UnitRecruit::setUnitType(core::UnitType *unitType)
{
    if (this->unitType != unitType)
    {
        wInfo(loggerName) << "UnitType changed " << this->unitType
            << " -> " << unitType;

        this->unitType = unitType;

        this->selectFocusNode();
        this->updateCanRecruit();

        emit unitTypeChanged();
    }
}

QString UnitRecruit::getError() const
{
    return this->error;
}

bool UnitRecruit::getCanRecruit() const
{
    return this->canRecruit;
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

void UnitRecruit::updateCanRecruit()
{
    bool canRecruit = this->settlement != nullptr
        && this->focusedNode != nullptr
        && this->unitType != nullptr
        && this->game->canRecruitUnit(this->unitType, this->focusedNode);

    if (this->canRecruit != canRecruit)
    {
        this->canRecruit = canRecruit;
        emit canRecruitChanged();
    }
}

void UnitRecruit::recruitUnit()
{
    if (!this->canRecruit)
        return;

    // Clear the error
    this->setError(QString());

    try
    {
        this->game->recruitUnit(
            this->unitType,
            this->focusedNode
        );
    }
    catch(core::UnitRecruitError &e)
    {
        wError(loggerName) << "Error recruiting unit: " << e;
        this->setError(e.getMessage());
    }

    this->selectFocusNode();
}

void UnitRecruit::mousePressEvent(QMouseEvent *event)
{
    this->updateFocus(event->pos());
}

void UnitRecruit::setError(const QString &error)
{
    if (this->error != error)
    {
        this->error = error;
        emit errorChanged();
    }
}

void UnitRecruit::onUnitAdded(const core::Unit *unit)
{
    Q_UNUSED(unit);
    this->update();
}

void UnitRecruit::cleanup()
{
    if (this->mapDrawer != nullptr)
        delete this->mapDrawer;

    this->nodes.clear();
    this->nodesPos.clear();
}

void UnitRecruit::setupMap()
{
    this->cleanup();

    QList<core::MapNode *> nodes;

    core::MapNode *node = settlement->getMapNode();
    nodes << node;
    nodes << node->getNeighbours().values();

    this->nodes = nodes;
    QHash<const core::MapNode *, QPoint> allNodesPos = positionNodes(
        node,
        this->tileSize
    );

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

void UnitRecruit::setFocusedNode(core::MapNode *focusedNode)
{
    if (focusedNode == nullptr
            || (
                this->canRecruitOnNode(focusedNode)
                && this->focusedNode != focusedNode
            )
        )
    {
        wInfo(loggerName) << "Focused node changed " << this->focusedNode
            << " -> " << focusedNode;
        this->focusedNode = focusedNode;

        this->updateCanRecruit();
        this->update();
    }
}

void UnitRecruit::updateFocus(const QPoint &p)
{
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
    this->setFocusedNode(focusedNode);
}

bool UnitRecruit::canRecruitOnNode(const core::MapNode *node) const
{
    bool nodeFree = !this->game->hasSettlement(node)
        && !this->game->hasUnit(node);

    if (this->unitType == nullptr)
    {
        return nodeFree;
    }
    else
    {
        core::UnitClass *klass = this->unitType->getClass();
        const int mc = klass->getMovementCost(node->getTerrainType());
        return nodeFree && (mc >= 0);
    }
}

void UnitRecruit::selectFocusNode()
{
    static const QList<core::MapNode::Direction> directionOrder{
        core::MapNode::West,
        core::MapNode::NorthWest,
        core::MapNode::NorthEast,
        core::MapNode::East,
        core::MapNode::SouthEast,
        core::MapNode::SouthWest
    };

    if (this->focusedNode != nullptr
            && this->canRecruitOnNode(this->focusedNode))
        return;

    int lastIndex = -1;
    core::MapNode *settlementNode = this->settlement->getMapNode();

    if (this->focusedNode != nullptr)
    {
        QHash<core::MapNode::Direction, core::MapNode *> nodes =
            settlementNode->getNeighbours();
        QHash<core::MapNode::Direction, core::MapNode *>::ConstIterator it;
        for (it = nodes.constBegin(); it != nodes.constEnd(); it++)
        {
            if (it.value() == this->focusedNode)
            {
                lastIndex = directionOrder.indexOf(it.key());
                break;
            }
        }
    }

    core::MapNode *nextFocusedNode = nullptr;

    int l = directionOrder.size();
    for (int i = (lastIndex + 1) % l; i  != lastIndex; i = (i + 1) % l)
    {
        core::MapNode::Direction direction = directionOrder[i];
        core::MapNode *node = settlementNode->getNeighbour(direction);
        if (node != nullptr && this->canRecruitOnNode(node))
        {
            nextFocusedNode = node;
            break;
        }
    }

    this->setFocusedNode(nextFocusedNode);
}
