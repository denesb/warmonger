#include <QSGSimpleTextureNode>

#include "ui/CampaignMapEditor.h"
#include "ui/MapUtil.h"
#include "utils/Constants.h"
#include "utils/Logging.h"

namespace warmonger {
namespace ui {

CampaignMapEditor::CampaignMapEditor(QQuickItem *parent) :
    BasicMap(parent),
    campaignMap(nullptr),
    worldSurface(nullptr),
    currentMapNode(nullptr)
{
    this->setAcceptHoverEvents(true);
}

core::CampaignMap* CampaignMapEditor::getCampaignMap() const
{
    return this->campaignMap;
}

void CampaignMapEditor::setCampaignMap(core::CampaignMap* campaignMap)
{
    if (this->campaignMap != campaignMap)
    {
        wInfo << "campaignMap `" << this->campaignMap << "' -> `" << campaignMap << "'";

        this->campaignMap = campaignMap;
        this->updateContent();

        emit campaignMapChanged();
    }
}

WorldSurface* CampaignMapEditor::getWorldSurface() const
{
    return this->worldSurface;
}

void CampaignMapEditor::setWorldSurface(WorldSurface* worldSurface)
{
    if (this->worldSurface != worldSurface)
    {
        wInfo << "worldSurface `" << this->worldSurface << "' -> `" << worldSurface << "'";

        this->worldSurface = worldSurface;
        this->updateContent();

        emit worldSurfaceChanged();
    }
}

QSGNode* CampaignMapEditor::updatePaintNode(QSGNode *oldRootNode, UpdatePaintNodeData *)
{
    const QMatrix4x4 transform = ui::moveTo(this->getWindowRect().topLeft(), QPoint(0, 0));

    QSGClipNode* rootNode;
    QSGTransformNode* mapRootNode;
    if (oldRootNode == nullptr)
    {
        rootNode = new QSGClipNode();
        rootNode->setIsRectangular(true);

        mapRootNode = new QSGTransformNode();
        mapRootNode->setMatrix(transform);

        rootNode->appendChildNode(mapRootNode);
    }
    else
    {
        rootNode = static_cast<QSGClipNode*>(oldRootNode);

        mapRootNode = static_cast<QSGTransformNode*>(rootNode->firstChild());
        if (transform != mapRootNode->matrix())
        {
            mapRootNode->setMatrix(transform);
        }
    }

    rootNode->setClipRect(QRectF(0, 0, this->width(), this->height()));

    const std::vector<const core::MapNode*> mapNodes = visibleMapNodes(
            this->mapNodesPos,
            this->worldSurface->getTileSize(),
            this->getWindowRect());

    drawMapNodes(mapNodes, mapRootNode, *this);

    return rootNode;
}

QSGNode* CampaignMapEditor::drawMapNodeAndContents(const core::MapNode* mapNode, QSGNode* oldNode)
{
    QSGNode* mapNodeSGNode = drawMapNode(mapNode, this->worldSurface, this->mapNodesPos.at(mapNode), oldNode);
    if(this->currentMapNode == mapNode)
    {
        if(mapNodeSGNode->firstChild() == nullptr)
        {
            QSGSimpleTextureNode* node = new QSGSimpleTextureNode();
            QSGTexture* texture = worldSurface->getTexture(utils::resourcePaths::mapEditor::hoverValid);

            node->setRect(static_cast<QSGSimpleTextureNode*>(mapNodeSGNode)->rect());
            node->setTexture(texture);

            mapNodeSGNode->appendChildNode(node);
        }
    }
    else
    {
        if(mapNodeSGNode->firstChild() != nullptr)
        {
            mapNodeSGNode->removeAllChildNodes();
        }
    }

    return mapNodeSGNode;
}

void CampaignMapEditor::hoverMoveEvent(QHoverEvent* event)
{
    core::MapNode* nextCurrentMapNode = mapNodeAtPos(
            event->pos(),
            this->campaignMap->getMapNodes(),
            this->mapNodesPos,
            this->worldSurface);

    if (this->currentMapNode != nextCurrentMapNode)
    {
        this->currentMapNode = nextCurrentMapNode;
        this->update();
    }
}

void CampaignMapEditor::updateContent()
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
    }
}

void CampaignMapEditor::updateMapRect()
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

} // namespace ui
} // namespace warmonger
