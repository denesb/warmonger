#include <algorithm>
#include <functional>
#include <iterator>

#include <QColor>
#include <QSGSimpleRectNode>

#include "core/Faction.h"
#include "core/Settlement.h"
#include "core/SettlementType.h"
#include "core/TerrainType.h"
#include "core/Unit.h"
#include "ui/MiniMap.h"
#include "log/LogStream.h"

static const QString loggerName{"ui.MiniMap"};

using namespace warmonger;
using namespace warmonger::ui;

MiniMap::MiniMap(QQuickItem *parent) :
    QQuickItem(parent),
    world(nullptr),
    worldSurface(nullptr),
    tileSize(),
    campaignMap(nullptr),
    windowPos(0, 0),
    windowSize(0, 0),
    scale(1.0),
    translate(0.0, 0.0)
{
    //this->setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);

    /*
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
    */
}

core::CampaignMap * MiniMap::getCampaignMap() const
{
    return this->campaignMap;
}

void MiniMap::setCampaignMap(core::CampaignMap *campaignMap)
{
    if (this->campaignMap != campaignMap)
    {
        /*
        if (this->campaignMap != nullptr)
            QObject::disconnect(
                this->campaignMap,
                nullptr,
                this,
                nullptr
            );
            */

        wInfo(loggerName) << "Map `" << this->campaignMap << "' -> `" << campaignMap << "'";

        this->campaignMap = campaignMap;
        this->setupMap();

        /*
        QObject::connect(
            this->campaignMap,
            &core::CampaignMap::unitsChanged,
            this,
            &QQuickItem::update
        );
        */

        this->updateContent();
        emit campaignMapChanged();
    }
}

WorldSurface * MiniMap::getWorldSurface() const
{
    return this->worldSurface;
}

void MiniMap::setWorldSurface(WorldSurface *worldSurface)
{
    if(this->worldSurface != worldSurface)
    {
        this->worldSurface = worldSurface;
        this->setupMap();

        this->updateContent();
        emit worldSurfaceChanged();
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
        //this->updateWindowPosRect();

        emit windowSizeChanged();
    }
}

QSGNode * MiniMap::updatePaintNode(QSGNode *, UpdatePaintNodeData *)
{
    wDebug(loggerName) << "updatePaintNode";
    QSGSimpleRectNode *n = new QSGSimpleRectNode();
    n->setColor(Qt::red);
    n->setRect(QRect(QPoint(0, 0), this->worldSurface->getTileSize()));
    return n;

    /*
    painter->scale(this->scale, this->scale);
    painter->translate(this->translate);
    */
}

/*
void MiniMap::mousePressEvent(QMouseEvent *event)
{
    QPointF pos(this->campaignMapToMap(event->pos()));
    this->centerWindow(pos.toPoint());
}

void MiniMap::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        QPointF pos(this->campaignMapToMap(event->pos()));
        this->centerWindow(pos.toPoint());
    }
}
*/

void MiniMap::updateContent()
{
    if (this->worldSurface == nullptr || this->campaignMap == nullptr || this->campaignMap->getMapNodes().empty())
    {
        wDebug(loggerName) << "doesn't has contents";
        this->setFlags(0);
    }
    else
    {
        wDebug(loggerName) << "has contents";
        this->setFlags(QQuickItem::ItemHasContents);
        this->update();
    }
}

void MiniMap::setupMap()
{
    if (this->worldSurface == nullptr || this->campaignMap == nullptr || this->nodes.empty())
    {
        return;
    }

    // these are used a lot
    this->nodes = this->campaignMap->getMapNodes();
    this->world = this->campaignMap->getWorld();

    this->tileSize = this->worldSurface->getTileSize();

    //this->hexagonPainterPath = hexagonPath(this->tileSize);
    this->nodesPos = positionNodes(this->nodes[0], this->tileSize);

    //this->updateGeometry();
}

/*
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
    std::pair<qreal, QPointF> transform = centerIn(
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
*/

/*
void MiniMap::drawNode(QPainter *, const core::MapNode *)
{
    const QString terrainTypeName = node->getTerrainType()->objectName();
    const core::Settlement *settlement = this->campaignMap->getSettlementOn(node);
    const core::Unit *unit = this->campaignMap->getUnitOn(node);

    painter->save();
    painter->translate(this->nodesPos[node]);

    const QColor color = this->worldSurface->getColor(terrainTypeName);
    painter->fillPath(this->hexagonPainterPath, color);

    const int w = this->tileSize.width();
    const int h = this->tileSize.height();

    if (settlement != nullptr)
    {
        const int size = w/2 - w/10;
        const QRect sr(w/20, h/2 - size/2, size, size);

        const core::Faction *owner = settlement->getOwner();
        QColor sc;
        if (owner == nullptr)
            sc= this->worldSurface->getColorName("noOwner");
        else
            sc = owner->getColor();

        painter->fillRect(sr, sc);
    }

    if (unit != nullptr)
    {
        const int size = w/4;
        const QRect ur(w/2 + w/5, h/2 - size/2, size, size);

        const core::Faction *owner = unit->getOwner();
        QColor uc;
        if (owner == nullptr)
            uc= this->worldSurface->getColorName("noOwner");
        else
            uc = owner->getColor();

        painter->fillRect(ur, uc);
    }

    painter->restore();
}
    */
