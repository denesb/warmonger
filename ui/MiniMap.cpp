#include <algorithm>
#include <functional>
#include <iterator>

#include <QColor>
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
    worldSurface(nullptr),
    campaignMap(nullptr)
{
    //this->setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);

    QObject::connect(this, &MiniMap::widthChanged, this, &MiniMap::updateTransform);
    QObject::connect(this, &MiniMap::heightChanged, this, &MiniMap::updateTransform);

    QObject::connect(&this->mapWindow, &MapWindow::windowRectChanged, this, &QQuickItem::update);
}

core::CampaignMap* MiniMap::getCampaignMap() const
{
    return this->campaignMap;
}

void MiniMap::setCampaignMap(core::CampaignMap* campaignMap)
{
    if (this->campaignMap != campaignMap)
    {
        wInfo << "campaignMap `" << this->campaignMap << "' -> `" << campaignMap << "'";

        this->campaignMap = campaignMap;

        this->updateContent();
        emit campaignMapChanged();
    }
}

WorldSurface* MiniMap::getWorldSurface() const
{
    return this->worldSurface;
}

void MiniMap::setWorldSurface(WorldSurface* worldSurface)
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
    return this->viewWindowRect;
}

void MiniMap::setWindowRect(const QRect& windowRect)
{
    if (this->viewWindowRect != windowRect)
    {
        this->viewWindowRect = windowRect;
        emit windowRectChanged();

        this->update();
    }
}

QSGNode* MiniMap::updatePaintNode(QSGNode* oldRootNode, UpdatePaintNodeData*)
{
    QSGTransformNode* rootNode;
    QSGNode* mapRootNode;

    if (oldRootNode != nullptr)
    {
        rootNode = static_cast<QSGTransformNode*>(oldRootNode);
        mapRootNode = rootNode->firstChild();

        drawViewWindowRect(this->viewWindowRect, rootNode->lastChild());
    }
    else
    {
        rootNode = new QSGTransformNode();
        mapRootNode = new QSGNode;

        rootNode->appendChildNode(mapRootNode);
        rootNode->appendChildNode(drawViewWindowRect(this->viewWindowRect, nullptr));
    }

    if (this->transform != rootNode->matrix())
    {
        rootNode->setMatrix(this->transform);
    }

    wDebug << this->viewWindowRect;

    // ugh
    const std::vector<core::MapNode*> mapNodes = this->campaignMap->getMapNodes();
    std::vector<const core::MapNode*> visibleMapNodes;
    std::copy(mapNodes.cbegin(), mapNodes.cend(), std::back_inserter(visibleMapNodes));

    drawMapNodes(visibleMapNodes, mapRootNode, *this);

    return rootNode;
}

QSGNode* MiniMap::drawMapNodeAndContents(const core::MapNode* mapNode, QSGNode* oldNode)
{
    return drawMapNode(mapNode, this->worldSurface, this->mapNodesPos.at(mapNode), oldNode);
}

/*
void MiniMap::mousePressEvent(QMouseEvent* event)
{
    QPointF pos(this->campaignMapToMap(event->pos()));
    this->centerWindow(pos.toPoint());
}

void MiniMap::mouseMoveEvent(QMouseEvent* event)
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
        this->setFlags(0);
    }
    else
    {
        this->setFlags(QQuickItem::ItemHasContents);

        this->mapNodesPos = positionMapNodes(this->campaignMap->getMapNodes()[0], this->worldSurface->getTileSize());

        this->updateTransform();

        this->update();
    }
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
    if (this->worldSurface == nullptr || this->campaignMap == nullptr || this->campaignMap->getMapNodes().empty())
        return;

    this->mapWindow.setMapRect(calculateBoundingRect(this->mapNodesPos, this->worldSurface->getTileSize()));

    if (this->height() <= 0 || this->width() <= 0)
        return;

    const QRectF frame(0.0, 0.0, this->width(), this->height());

    this->transform = centerIn(this->mapWindow.getMapRect(), frame);

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
    painter->translate(this->mapNodesPos[node]);

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
