#include <algorithm>
#include <functional>
#include <iterator>

#include <QColor>
#include <QPainter>

#include "core/Map.h"
#include "core/Player.h"
#include "core/Settlement.h"
#include "core/SettlementType.h"
#include "core/TerrainType.h"
#include "core/Unit.h"
#include "core/Util.h"
#include "core/WorldSurface.h"
#include "ui/MapPreview.h"

static const QString category{"ui"};

static qreal calculateScaleFactor(const QSizeF &source, const QSizeF &target);

using namespace warmonger;
using namespace warmonger::ui;

MapPreview::MapPreview(QQuickItem *parent) :
    QQuickPaintedItem(parent),
    nodes(),
    world(nullptr),
    surface(nullptr),
    tileSize(),
    map(nullptr),
    nodesInfo(),
    boundingRect(),
    hexagonPainterPath(),
    scale(1.0),
    translate(0.0, 0.0)
{
    QObject::connect(
        this,
        &MapPreview::widthChanged,
        this,
        &MapPreview::updateTransform
    );

    QObject::connect(
        this,
        &MapPreview::heightChanged,
        this,
        &MapPreview::updateTransform
    );
}

MapPreview::~MapPreview()
{
}

core::Map * MapPreview::getMap() const
{
    return this->map;
}

void MapPreview::setMap(core::Map *map)
{
    if (this->map != map)
    {
        this->map = map;
        this->setupMap();
        emit mapChanged();
    }
}

QObject * MapPreview::readMap() const
{
    return this->map;
}

void MapPreview::writeMap(QObject *map)
{
    if (map == nullptr)
    {
        this->setMap(nullptr);
        return;
    }

    core::Map *m = qobject_cast<core::Map *>(map);
    if (m == nullptr)
    {
        wError(category) << "map has wrong type";
        throw core::Exception(core::Exception::InvalidValue);
    }
    this->setMap(m);
}

void MapPreview::paint(QPainter *painter)
{
    if (this->map == nullptr)
        return;

    painter->save();

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->scale(this->scale, this->scale);
    painter->translate(this->translate);

    auto cbegin = this->nodes.constBegin();
    auto cend = this->nodes.constEnd();
    std::function<void(const core::MapNode *)> drawNodeFunc = std::bind(
        &MapPreview::drawNode,
        this,
        painter,
        std::placeholders::_1
    );

    std::for_each(cbegin, cend, drawNodeFunc);

    painter->restore();
}

void MapPreview::setupMap()
{
    if (this->map == nullptr)
    {
        this->nodes = QList<core::MapNode *>();
        this->world = nullptr;
        this->surface = nullptr;
        this->tileSize = QSize();
        this->hexagonPainterPath = QPainterPath();
        this->nodesInfo.clear();
        this->boundingRect = QRect();

        this->update();
        return;
    }

    // these are used a lot
    this->nodes = this->map->getMapNodes();
    this->world = this->map->getWorld();
    this->surface = this->world->getSurface();
    this->tileSize = this->surface->getTileSize();

    this->hexagonPainterPath = hexagonPath(this->tileSize);

    this->nodesInfo.clear();
    this->nodesInfo.insert(this->nodes[0], new NodeInfo(this->nodes[0]));
    positionNodes(this->nodes[0], this->nodesInfo, this->tileSize);

    const QList<core::Settlement *> settlements = this->map->getSettlements();
    std::for_each(
        settlements.constBegin(),
        settlements.constEnd(),
        [&](core::Settlement *s)
        {
            //FIXME: can settlement's mapnodes be null?
            this->nodesInfo[s->getMapNode()]->settlement = s;
        }
    );

    const QList<core::Unit *> units = this->map->getUnits();
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
    this->update();
}

void MapPreview::updateGeometry()
{
    this->boundingRect = calculateBoundingRect(
        this->nodesInfo,
        this->tileSize
    );
    this->updateTransform();
}

void MapPreview::updateTransform()
{
    this->scale = calculateScaleFactor(
        QSizeF(this->boundingRect.size()),
        this->contentsBoundingRect().size()
    );

    const QSizeF itemSize = this->contentsBoundingRect().size();
    const QSizeF mapSize = QSizeF(this->boundingRect.size()) * this->scale;
    const qreal dx = (itemSize.width() - mapSize.width()) / 2.0;
    const qreal dy = (itemSize.height() - mapSize.height()) / 2.0;

    this->translate = QPointF(-this->boundingRect.topLeft());
    this->translate += (QPointF(dx, dy) * (1 / this->scale));
}

void MapPreview::drawNode(QPainter *painter, const core::MapNode *node)
{
    const NodeInfo *nodeInfo = this->nodesInfo[node];
    const QString terrainTypeName = node->getTerrainType()->objectName();
    const core::Settlement *settlement = nodeInfo->settlement;
    const core::Unit *unit = nodeInfo->unit;

    painter->save();
    painter->translate(nodeInfo->pos);

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

qreal calculateScaleFactor(const QSizeF &source, const QSizeF &target)
{
    const qreal sourceSize = std::max(source.width(), source.height());
    const qreal targetSize = std::min(target.width(), target.width());

    return targetSize / sourceSize;
}
