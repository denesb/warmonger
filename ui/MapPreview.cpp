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
#include "core/WorldSurface.h"
#include "ui/MapPreview.h"

using namespace warmonger;
using namespace warmonger::ui;

MapPreview::MapPreview(QQuickItem* parent)
    : QQuickPaintedItem(parent)
    , nodes()
    , world(nullptr)
    , surface(nullptr)
    , tileSize()
    , map(nullptr)
    , nodesPos()
    , boundingRect()
    , hexagonPainterPath()
    , scale(1.0)
    , translate(0.0, 0.0)
{
    QObject::connect(this, &MapPreview::widthChanged, this, &MapPreview::updateTransform);

    QObject::connect(this, &MapPreview::heightChanged, this, &MapPreview::updateTransform);
}

MapPreview::~MapPreview()
{
}

core::Map* MapPreview::getMap() const
{
    return this->map;
}

void MapPreview::setMap(core::Map* map)
{
    if (this->map != map)
    {
        this->map = map;
        this->setupMap();
        emit mapChanged();
    }
}

void MapPreview::paint(QPainter* painter)
{
    if (this->map == nullptr)
        return;

    painter->save();

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->scale(this->scale, this->scale);
    painter->translate(this->translate);

    auto cbegin = this->nodes.constBegin();
    auto cend = this->nodes.constEnd();
    std::function<void(const core::MapNode*)> drawNodeFunc =
        std::bind(&MapPreview::drawNode, this, painter, std::placeholders::_1);

    std::for_each(cbegin, cend, drawNodeFunc);

    painter->restore();
}

void MapPreview::setupMap()
{
    if (this->map == nullptr)
    {
        this->nodes = QList<core::MapNode*>();
        this->world = nullptr;
        this->surface = nullptr;
        this->tileSize = QSize();
        this->hexagonPainterPath = QPainterPath();
        this->nodesPos.clear();
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
    this->nodesPos = positionNodes(this->nodes[0], this->tileSize);

    this->updateGeometry();
    this->update();
}

void MapPreview::updateGeometry()
{
    this->boundingRect = calculateBoundingRect(this->nodesPos, this->tileSize);
    this->updateTransform();
}

void MapPreview::updateTransform()
{
    QPair<qreal, QPointF> transform = centerIn(this->boundingRect, this->contentsBoundingRect());
    this->scale = transform.first;
    this->translate = transform.second;
}

void MapPreview::drawNode(QPainter* painter, const core::MapNode* node)
{
    const QString terrainTypeName = node->getTerrainType()->objectName();
    const core::Settlement* settlement = this->map->getSettlementOn(node);
    const core::Unit* unit = this->map->getUnitOn(node);

    painter->save();
    painter->translate(this->nodesPos[node]);

    const QColor color = this->surface->getColor(terrainTypeName);
    painter->fillPath(this->hexagonPainterPath, color);

    const int w = this->tileSize.width();
    const int h = this->tileSize.height();

    if (settlement != nullptr)
    {
        const int size = w / 2 - w / 10;
        const QRect sr(w / 20, h / 2 - size / 2, size, size);

        const core::Player* owner = settlement->getOwner();
        QColor sc;
        if (owner == nullptr)
            sc = this->surface->getColorName("noOwner");
        else
            sc = owner->getColor();

        painter->fillRect(sr, sc);
    }

    if (unit != nullptr)
    {
        const int size = w / 4;
        const QRect ur(w / 2 + w / 5, h / 2 - size / 2, size, size);

        const core::Player* owner = unit->getOwner();
        QColor uc;
        if (owner == nullptr)
            uc = this->surface->getColorName("noOwner");
        else
            uc = owner->getColor();

        painter->fillRect(ur, uc);
    }

    painter->restore();
}
