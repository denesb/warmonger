#include <QColor>
#include <QSGTransformNode>

#include "core/Faction.h"
#include "core/Settlement.h"
#include "core/SettlementType.h"
#include "core/TerrainType.h"
#include "core/Unit.h"
#include "ui/CampaignMiniMap.h"
#include "ui/CampaignMapWatcher.h"
#include "ui/MapUtil.h"
#include "utils/Logging.h"

namespace warmonger {
namespace ui {

CampaignMiniMap::CampaignMiniMap(QQuickItem *parent) :
    BasicMiniMap(parent),
    worldSurface(nullptr),
    campaignMap(nullptr),
    watcher(nullptr)
{
}

core::CampaignMap* CampaignMiniMap::getCampaignMap() const
{
    return this->campaignMap;
}

void CampaignMiniMap::setCampaignMap(core::CampaignMap* campaignMap)
{
    if (this->campaignMap != campaignMap)
    {
        wInfo << "campaignMap `" << this->campaignMap << "' -> `" << campaignMap << "'";

        this->campaignMap = campaignMap;

        this->updateContent();

        delete this->watcher;
        this->watcher = new CampaignMapWatcher(this->campaignMap, this);
        QObject::connect(this->watcher, &CampaignMapWatcher::changed, this, &CampaignMiniMap::update);

        emit campaignMapChanged();
    }
}

WorldSurface* CampaignMiniMap::getWorldSurface() const
{
    return this->worldSurface;
}

void CampaignMiniMap::setWorldSurface(WorldSurface* worldSurface)
{
    if(this->worldSurface != worldSurface)
    {
        wInfo << "worldSurface `" << this->worldSurface << "' -> `" << worldSurface << "'";

        this->worldSurface = worldSurface;

        this->updateContent();
        emit worldSurfaceChanged();
    }
}

QSGNode* CampaignMiniMap::updatePaintNode(QSGNode* oldRootNode, UpdatePaintNodeData*)
{
    QSGTransformNode* rootNode;
    QSGNode* mapRootNode;

    if (oldRootNode != nullptr)
    {
        rootNode = static_cast<QSGTransformNode*>(oldRootNode);
        mapRootNode = rootNode->firstChild();

        drawRect(this->getWindowRect(), rootNode->lastChild());
    }
    else
    {
        rootNode = new QSGTransformNode();
        mapRootNode = new QSGNode;

        rootNode->appendChildNode(mapRootNode);
        rootNode->appendChildNode(drawRect(this->getWindowRect(), nullptr));
    }

    const QMatrix4x4& transform = this->getTransformMatrix();
    if (transform != rootNode->matrix())
    {
        rootNode->setMatrix(transform);
    }

    // ugh
    const std::vector<core::MapNode*> mapNodes = this->campaignMap->getMapNodes();
    std::vector<const core::MapNode*> visibleMapNodes;
    std::copy(mapNodes.cbegin(), mapNodes.cend(), std::back_inserter(visibleMapNodes));

    drawMapNodes(visibleMapNodes, mapRootNode, *this);

    return rootNode;
}

QSGNode* CampaignMiniMap::drawMapNodeAndContents(const core::MapNode* mapNode, QSGNode* oldNode)
{
    return drawMapNode(mapNode, this->worldSurface, this->mapNodesPos.at(mapNode), oldNode);
}

void CampaignMiniMap::updateContent()
{
    if (this->worldSurface == nullptr || this->campaignMap == nullptr || this->campaignMap->getMapNodes().empty()
            || this->worldSurface->getWorld() != this->campaignMap->getWorld())
    {
        this->setFlags(0);
    }
    else
    {
        this->setFlags(QQuickItem::ItemHasContents);

        this->mapNodesPos = positionMapNodes(this->campaignMap->getMapNodes()[0], this->worldSurface->getTileSize());

        this->updateMapRect();

        this->update();
    }
}

void CampaignMiniMap::updateMapRect()
{
    if (this->worldSurface == nullptr || this->campaignMap == nullptr || this->campaignMap->getMapNodes().empty()
            || this->worldSurface->getWorld() != this->campaignMap->getWorld())
    {
        this->setMapRect(QRect(0, 0, 0, 0));
    }
    else
    {
        this->setMapRect(calculateBoundingRect(this->mapNodesPos, this->worldSurface->getTileSize()));
        this->update();
    }
}

/*
void CampaignMiniMap::drawNode(QPainter *, const core::MapNode *)
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
