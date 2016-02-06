#include <algorithm>
#include <functional>
#include <iterator>

#include <QBrush>
#include <QColor>
#include <QRect>

#include "core/Map.h"
#include "core/Settlement.h"
#include "core/SettlementType.h"
#include "core/TerrainType.h"
#include "core/Unit.h"
#include "core/UnitType.h"
#include "core/WorldSurface.h"
#include "ui/MapDrawer.h"
#include "ui/MapUtil.h"

using namespace warmonger;
using namespace warmonger::ui;

MapDrawer::MapDrawer(
    core::Map *map,
    const QHash<const core::MapNode *, QPoint> &nodesPos
) :
    map(map),
    world(nullptr),
    surface(nullptr),
    nodesPos(nodesPos),
    hexagonPainterPath()
{
    this->world = this->map->getWorld();
    this->surface = this->world->getSurface();
    this->tileSize = this->surface->getTileSize();
    this->hexagonPainterPath = hexagonPath(this->tileSize);
}

MapDrawer::~MapDrawer()
{
}

void MapDrawer::drawMap(
    QPainter *painter,
    const QList<core::MapNode *> &nodes,
    const DrawingInfo &drawingInfo
) const
{
    painter->save();

    this->drawNodes(painter, nodes);
    this->drawNodesGrid(painter, nodes);

    if (drawingInfo.focusedNode != nullptr)
        this->drawFocusMark(painter, drawingInfo.focusedNode);

    this->drawNodesSettlements(painter, nodes);
    this->drawOverlays(painter, nodes, drawingInfo.overlays);

    this->drawUnits(painter, nodes, drawingInfo.movingUnits);

    painter->restore();
}

void MapDrawer::drawNodes(
    QPainter *painter,
    const QList<core::MapNode *> &nodes
) const
{
    std::function<void(const core::MapNode *)> drawFunc = std::bind(
        &MapDrawer::drawNode,
        this,
        painter,
        std::placeholders::_1
    );
    std::for_each(nodes.constBegin(), nodes.constEnd(), drawFunc);
}

void MapDrawer::drawNode(QPainter *painter, const core::MapNode *node) const
{
    const QString terrainTypeName = node->getTerrainType()->objectName();
    QImage image = this->surface->getImage(terrainTypeName);
    QPoint pos = this->nodesPos[node];
    painter->drawImage(pos, image);
}

void MapDrawer::drawNodesGrid(
    QPainter *painter,
    const QList<core::MapNode *> &nodes
) const
{
    std::function<void(const core::MapNode *)> drawFunc = std::bind(
        &MapDrawer::drawNodeGrid,
        this,
        painter,
        std::placeholders::_1
    );
    std::for_each(nodes.constBegin(), nodes.constEnd(), drawFunc);
}

void MapDrawer::drawNodeGrid(
    QPainter *painter,
    const core::MapNode *node
) const
{
    const QPoint pos = this->nodesPos[node];
    const QColor color = this->surface->getColor("grid");
    QPen pen(color);
    pen.setWidth(1);

    painter->save();
    painter->translate(pos);

    painter->strokePath(this->hexagonPainterPath, pen);

    painter->restore();
}

void MapDrawer::drawFocusMark(
    QPainter *painter,
    const core::MapNode *node
) const
{
    const QPoint pos = this->nodesPos[node];
    const QColor color = this->surface->getColor("focusOutline");
    QPen pen(color);
    pen.setWidth(2);

    painter->save();
    painter->translate(pos);

    painter->strokePath(this->hexagonPainterPath, pen);

    painter->restore();
}

void MapDrawer::drawNodesSettlements(
    QPainter *painter,
    const QList<core::MapNode *> &nodes
) const
{
    for (core::MapNode *node : nodes)
    {
        if (this->map->hasSettlement(node))
            this->drawSettlement(painter, this->map->getSettlementOn(node));
    }
}

void MapDrawer::drawSettlement(
    QPainter *painter,
    const core::Settlement *settlement
) const
{
    const QPoint pos = this->nodesPos[settlement->getMapNode()];
    const QRect frame(pos, this->tileSize);
    const core::SettlementType *st = settlement->getType();
    const QImage image = this->surface->getImage(st->objectName());

    painter->drawImage(frame, image);
}

void MapDrawer::drawOverlays(
    QPainter *painter,
    const QList<core::MapNode *> &nodes,
    const QList<DrawingInfo::Overlay> &overlays
) const
{
    std::function<void(const DrawingInfo::Overlay &)> drawFunc = std::bind(
        &MapDrawer::drawOverlay,
        this,
        painter,
        nodes,
        std::placeholders::_1
    );
    std::for_each(overlays.constBegin(), overlays.constEnd(), drawFunc);
}

void MapDrawer::drawOverlay(
    QPainter *painter,
    const QList<core::MapNode *> &nodes,
    const DrawingInfo::Overlay &overlay
) const
{
    for (core::MapNode *node : nodes)
    {
        if (overlay.nodes.contains(node))
            this->drawNodeOverlay(painter, overlay.color, node);
    }
}

void MapDrawer::drawNodeOverlay(
    QPainter *painter,
    QColor color,
    const core::MapNode *node
) const
{
    const QBrush brush(color);

    painter->save();
    painter->translate(this->nodesPos[node]);

    painter->fillPath(this->hexagonPainterPath, brush);

    painter->restore();
}

void MapDrawer::drawUnits(
    QPainter *painter,
    const QList<core::MapNode *> &nodes,
    QHash<const core::Unit *, QPoint> movingUnits
) const
{
    for (core::MapNode *node : nodes)
    {
        core::Unit *unit = this->map->getUnitOn(node);
        if (unit != nullptr && !movingUnits.contains(unit))
            this->drawUnit(painter, this->nodesPos[node], unit);
    }

    QHash<const core::Unit *, QPoint>::ConstIterator it;
    for (it = movingUnits.constBegin(); it != movingUnits.constEnd(); it++)
    {
        this->drawUnit(painter, it.value(), it.key());
    }
}

void MapDrawer::drawUnit(
    QPainter *painter,
    const QPoint &pos,
    const core::Unit *unit
) const
{
    const QRect frame(pos, this->tileSize);
    const core::UnitType *ut = unit->getType();
    const QImage image = this->surface->getImage(ut->objectName());

    painter->drawImage(frame, image);
}
