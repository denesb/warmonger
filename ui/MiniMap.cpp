#include <algorithm>
#include <functional>
#include <iterator>

#include <QColor>
#include <QSGSimpleRectNode>
#include <QSGSimpleTextureNode>
#include <QSGTransformNode>

#include "core/Faction.h"
#include "core/Settlement.h"
#include "core/SettlementType.h"
#include "core/TerrainType.h"
#include "core/Unit.h"
#include "ui/MiniMap.h"
#include "utils/Logging.h"

namespace warmonger {
namespace ui {

MiniMap::MiniMap(QQuickItem *parent) :
    QQuickItem(parent),
    world(nullptr),
    worldSurface(nullptr),
    campaignMap(nullptr),
    mapWindow(QRect(), QSize()),
    transformChanged(true),
    contentChanged(true)
{
    //this->setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);

    QObject::connect(this, &MiniMap::widthChanged, this, &MiniMap::updateTransform);
    QObject::connect(this, &MiniMap::heightChanged, this, &MiniMap::updateTransform);

    QObject::connect(&this->mapWindow, &MapWindow::windowRectChanged, this, &QQuickItem::update);
    QObject::connect(&this->mapWindow, &MapWindow::windowRectChanged, this, &MiniMap::windowRectChanged);
}

core::CampaignMap * MiniMap::getCampaignMap() const
{
    return this->campaignMap;
}

void MiniMap::setCampaignMap(core::CampaignMap *campaignMap)
{
    if (this->campaignMap != campaignMap)
    {
        wInfo << "campaignMap `" << this->campaignMap << "' -> `" << campaignMap << "'";

        this->campaignMap = campaignMap;

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

        this->updateContent();
        emit worldSurfaceChanged();
    }
}

QRect MiniMap::getWindowRect() const
{
    return this->mapWindow.getWindowRect();
}

void MiniMap::setWindowRect(const QRect &windowPos)
{
    this->mapWindow.setWindowRect(windowPos);
}

QSGNode * MiniMap::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    wDebug << "updatePaintNode";

    QSGTransformNode *rootNode;
    if (oldNode != nullptr)
        rootNode = dynamic_cast<QSGTransformNode*>(oldNode);
    else
        rootNode = new QSGTransformNode();

    if (this->transformChanged)
    {
        rootNode->setMatrix(this->transform);
        this->transformChanged = false;
    }

    if (this->contentChanged)
    {
        this->contentChanged = false;
        rootNode->removeAllChildNodes();

        const std::vector<core::MapNode *> nodes = this->campaignMap->getMapNodes();
        for (core::MapNode *node : nodes)
        {
            QSGTexture *texture = this->worldSurface->getTexture(node->getTerrainType());
            if (texture == nullptr)
                continue;

            QSGSimpleTextureNode *n = new QSGSimpleTextureNode();
            n->setOwnsTexture(false);

            n->setRect(QRect(this->nodesPos[node], this->worldSurface->getTileSize()));
            n->setTexture(texture);

            rootNode->appendChildNode(n);
        }
    }

        /*
    QSGSimpleRectNode *n = new QSGSimpleRectNode();
    n->setColor(Qt::red);
    n->setRect(QRect(QPoint(0, 0), this->worldSurface->getTileSize()));
    */

    return rootNode;
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
        wDebug << "doesn't has contents";
        this->setFlags(0);
        return;
    }
    else
    {
        wDebug << "has contents";
        this->setFlags(QQuickItem::ItemHasContents);
        //this->update();
    }

    // these are used a lot
    this->nodes = this->campaignMap->getMapNodes();
    this->world = this->campaignMap->getWorld();

    this->tileSize = this->worldSurface->getTileSize();

    this->nodesPos = positionMapNodes(this->nodes[0], this->tileSize);

    this->contentChanged = true;

    this->updateGeometry();
    this->update();

    wDebug << "Content changed, schedule redraw";
}

void MiniMap::updateGeometry()
{
    this->mapWindow.setMapRect(calculateBoundingRect(this->nodesPos, this->tileSize));

    this->updateTransform();

    //this->updateWindowRectRect();
    //this->setWindowRect(this->boundingRect.topLeft());
}

/*
void MiniMap::updateWindowRectRect()
{
    this->windowPosRect = QRect(
        this->boundingRect.x(),
        this->boundingRect.y(),
        this->boundingRect.width() - this->windowSize.width(),
        this->boundingRect.height() - this->windowSize.height()
    );
}
*/

void MiniMap::updateTransform()
{
    if (this->height() <= 0 || this->width() <= 0)
        return;

    const QRectF frame(0.0, 0.0, this->width(), this->height());

    this->transform = centerIn(this->mapWindow.getMapRect(), frame);

    this->transformChanged = true;

    wDebug << "Transformation matrix changed, schedule redraw";

    this->update();
}

/*
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

} // namespace ui
} // namespace warmonger
