#include "ui/CampaignMapEditor.h"
#include "ui/MapUtil.h"
#include "utils/Logging.h"

namespace warmonger {
namespace ui {

CampaignMapEditor::CampaignMapEditor(QQuickItem *parent) :
    BasicMap(parent),
    campaignMap(nullptr),
    worldSurface(nullptr)
{
}

core::CampaignMap* CampaignMapEditor::getCampaignMap() const
{
    return this->campaignMap;
}

void CampaignMapEditor::setCampaignMap(core::CampaignMap* campaignMap)
{
    if (this->campaignMap != campaignMap)
    {
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
        this->worldSurface = worldSurface;
        this->updateContent();

        emit worldSurfaceChanged();
    }
}

QSGNode* CampaignMapEditor::updatePaintNode(QSGNode *oldRootNode, UpdatePaintNodeData *)
{
    QSGNode* rootNode;
    if (oldRootNode == nullptr)
    {
        rootNode = new QSGNode();
    }
    else
    {
        rootNode = oldRootNode;
    }

    const std::vector<const core::MapNode*> mapNodes = visibleMapNodes(
            this->mapNodesPos,
            this->worldSurface->getTileSize(),
            this->getWindowRect());

    drawMapNodes(mapNodes, rootNode, *this);

    return rootNode;
}

QSGNode* CampaignMapEditor::drawMapNodeAndContents(const core::MapNode* mapNode, QSGNode* oldNode)
{
    return drawMapNode(mapNode, this->worldSurface, this->mapNodesPos.at(mapNode), oldNode);
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
        this->update();
    }
}

} // namespace ui
} // namespace warmonger
