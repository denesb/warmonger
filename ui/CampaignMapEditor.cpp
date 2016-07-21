#include <QSGSimpleTextureNode>

#include "ui/CampaignMapEditor.h"
#include "ui/MapUtil.h"
#include "utils/Logging.h"

namespace warmonger {
namespace ui {

CampaignMapEditor::CampaignMapEditor(QQuickItem *parent) :
    QQuickItem(parent),
    campaignMap(nullptr)
{
    QObject::connect(this, &CampaignMapEditor::widthChanged, this, &CampaignMapEditor::updateGeometry);
    QObject::connect(this, &CampaignMapEditor::heightChanged, this, &CampaignMapEditor::updateGeometry);
    QObject::connect(&this->mapWindow, &MapWindow::windowRectChanged, this, &CampaignMapEditor::windowRectChanged);
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

QRect CampaignMapEditor::getWindowRect() const
{
    return this->mapWindow.getWindowRect();
}

QSGNode* CampaignMapEditor::updatePaintNode(QSGNode *oldRoot, UpdatePaintNodeData *)
{
    QSGNode* root;
    if (oldRoot == nullptr)
    {
        root = new QSGNode();
    }
    else
    {
        root = oldRoot;
    }

    const std::vector<const core::MapNode*> mapNodes = visibleMapNodes(
            this->mapNodesPos,
            this->worldSurface->getTileSize(),
            this->mapWindow.getWindowRect());

    const int mapNodesSize = static_cast<int>(mapNodes.size());
    const int nodesCount = root->childCount();

    const int n = std::min(mapNodesSize, nodesCount);

    for (int i = 0; i < n; ++i)
    {
        QSGNode* oldNode = root->childAtIndex(i);

        // returned node ignored, since it will always be oldNode
        this->drawNode(oldNode, mapNodes[i]);
    }

    if (mapNodesSize < nodesCount)
    {
        for (int i = mapNodesSize; i < nodesCount; ++i)
        {
            root->removeChildNode(root->childAtIndex(i));
        }
    }
    else if (mapNodesSize > nodesCount)
    {
        for (int i = nodesCount; i < mapNodesSize; ++i)
        {
            QSGNode* newNode = this->drawNode(nullptr, mapNodes[i]);
            root->appendChildNode(newNode);
        }
    }

    return root;
}

void CampaignMapEditor::updateContent()
{
    if (this->worldSurface == nullptr || this->campaignMap == nullptr || this->campaignMap->getMapNodes().empty()
            || this->worldSurface->getWorld() != this->campaignMap->getWorld())
    {
        this->setFlags(0);
        wDebug << "No Content";
    }
    else
    {
        this->setFlags(QQuickItem::ItemHasContents);
        this->mapNodesPos = positionMapNodes(this->campaignMap->getMapNodes()[0], this->worldSurface->getTileSize());
        this->update();
        wDebug << "Content";
    }
}

void CampaignMapEditor::updateGeometry()
{
    const QPoint topLeft = this->mapWindow.getWindowRect().topLeft();
    this->mapWindow.setWindowRect(QRect(topLeft.x(), topLeft.y(), this->width(), this->height()));
}

QSGNode* CampaignMapEditor::drawNode(QSGNode* oldNode, const core::MapNode* mapNode) const
{
    QSGSimpleTextureNode *node;
    if (oldNode == nullptr)
    {
        node = new QSGSimpleTextureNode();
        node->setOwnsTexture(false);
    }
    else
    {
        // if not nullptr, it can only be a texture node
        node = static_cast<QSGSimpleTextureNode*>(oldNode);
    }

    QSGTexture* texture = this->worldSurface->getTexture(mapNode->getTerrainType());
    if (texture == nullptr)
    {
        wError << "No texture found for " << mapNode->getTerrainType();
        //FIXME: Use the unknown texture here
    }

    QSGTexture* currentTexture = node->texture();

    if (currentTexture == nullptr || currentTexture->textureId() != texture->textureId())
    {
        node->setTexture(texture);
    }

    const QRect nodeRect(this->mapNodesPos.at(mapNode), this->worldSurface->getTileSize());
    if (node->rect() != nodeRect)
    {
        node->setRect(nodeRect);
    }

    return node;
}

} // namespace ui
} // namespace warmonger
