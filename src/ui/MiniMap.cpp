#include <algorithm>
#include <functional>
#include <iterator>

#include <QColor>
#include <QPainter>

#include "core/Player.h"
#include "core/Settlement.h"
#include "core/SettlementType.h"
#include "core/TerrainType.h"
#include "core/Unit.h"
#include "core/Util.h"
#include "core/WorldSurface.h"
#include "ui/MiniMap.h"

static const QString category{"ui"};

using namespace warmonger;
using namespace warmonger::ui;

MiniMap::MiniMap(QQuickItem *parent) :
    QQuickPaintedItem(parent),
    nodes(),
    world(nullptr),
    surface(nullptr),
    tileSize(),
    map(nullptr),
    nodesPos(),
    boundingRect(),
    hexagonPainterPath(),
    windowPos(0, 0),
    windowSize(0, 0),
    scale(1.0),
    translate(0.0, 0.0)
{
    this->setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);

    QObject::connect(
        this,
        &MiniMap::widthChanged,
        this,
        &MiniMap::updateTransform
    );

    QObject::connect(
        this,
        &MiniMap::heightChanged,
        this,
        &MiniMap::updateTransform
    );
}

MiniMap::~MiniMap()
{
}

core::Map * MiniMap::getMap() const
{
    return this->map;
}

void MiniMap::setMap(core::Map *map)
{
    if (this->map != map)
    {
        this->map = map;
        this->setupMap();
        emit mapChanged();
    }
}

QPoint MiniMap::getWindowPos() const
{
    return this->windowPos;
}

void MiniMap::setWindowPos(const QPoint &windowPos)
{
    QPoint wp(project(windowPos, this->windowPosRect));

    if (this->windowPos != wp)
    {
        this->windowPos = wp;
        this->update();
        emit windowPosChanged();
    }
}

void MiniMap::centerWindow(const QPoint &pos)
{
    QPointF p(pos);
    p -= QPointF(this->windowSize.width(), this->windowSize.height()) / 2.0;
    this->setWindowPos(p.toPoint());
}

QSize MiniMap::getWindowSize() const
{
    return this->windowSize;
}

void MiniMap::setWindowSize(const QSize &windowSize)
{
    if (this->windowSize != windowSize)
    {
        this->windowSize = windowSize;
        this->updateWindowPosRect();

        emit windowSizeChanged();
    }
}

void MiniMap::paint(QPainter *painter)
{
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing, true);

    painter->scale(this->scale, this->scale);
    painter->translate(this->translate);

    auto cbegin = this->nodes.constBegin();
    auto cend = this->nodes.constEnd();
    std::function<void(const core::MapNode *)> drawNodeFunc = std::bind(
        &MiniMap::drawNode,
        this,
        painter,
        std::placeholders::_1
    );

    std::for_each(cbegin, cend, drawNodeFunc);

    const QColor windowColor("black");
    QPen pen(windowColor);
    pen.setWidthF(1 / this->scale);

    const QRect window(this->windowPos, this->windowSize);

    painter->setPen(pen);
    painter->drawRect(window);

    painter->restore();
}

void MiniMap::mousePressEvent(QMouseEvent *event)
{
    QPointF pos(this->mapToMap(event->pos()));
    this->centerWindow(pos.toPoint());
}

void MiniMap::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        QPointF pos(this->mapToMap(event->pos()));
        this->centerWindow(pos.toPoint());
    }
}

void MiniMap::setupMap()
{
    // these are used a lot
    this->nodes = this->map->getMapNodes();
    this->world = this->map->getWorld();
    this->surface = this->world->getSurface();
    this->tileSize = this->surface->getTileSize();

    this->hexagonPainterPath = hexagonPath(this->tileSize);
    this->nodesPos = positionNodes(this->nodes[0], this->tileSize);

    this->updateGeometry();
}

void MiniMap::updateGeometry()
{
    this->boundingRect = calculateBoundingRect(
        this->nodesPos,
        this->tileSize
    );

    this->updateWindowPosRect();
    this->setWindowPos(this->boundingRect.topLeft());
}

void MiniMap::updateWindowPosRect()
{
    this->windowPosRect = QRect(
        this->boundingRect.x(),
        this->boundingRect.y(),
        this->boundingRect.width() - this->windowSize.width(),
        this->boundingRect.height() - this->windowSize.height()
    );
}

void MiniMap::updateTransform()
{
    QPair<qreal, QPointF> transform = centerIn(
        this->boundingRect,
        this->contentsBoundingRect()
    );
    this->scale = transform.first;
    this->translate = transform.second;
}


QPointF MiniMap::mapToMap(const QPointF &p)
{
    const qreal rscale = 1 / this->scale;
    return p * rscale - this->translate;
}

void MiniMap::drawNode(QPainter *painter, const core::MapNode *node)
{
    const QString terrainTypeName = node->getTerrainType()->objectName();
    const core::Settlement *settlement = this->map->getSettlementOn(node);
    const core::Unit *unit = this->map->getUnitOn(node);

    painter->save();
    painter->translate(this->nodesPos[node]);

    const QColor color = this->surface->getColor(terrainTypeName);
    painter->fillPath(this->hexagonPainterPath, color);

    const int w = this->tileSize.width();
    const int h = this->tileSize.height();

    if (settlement != nullptr)
    {
        const int size = w/2 - w/10;
        const QRect sr(w/20, h/2 - size/2, size, size);
        const QColor sc = settlement->getOwner()->getColor();

        painter->fillRect(sr, sc);
    }

    if (unit != nullptr)
    {
        const int size = w/4;
        const QRect ur(w/2 + w/5, h/2 - size/2, size, size);
        const QColor uc = unit->getOwner()->getColor();

        painter->fillRect(ur, uc);
    }

    painter->restore();
}
