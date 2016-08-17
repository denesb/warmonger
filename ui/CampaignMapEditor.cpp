#include <QSGSimpleTextureNode>

#include "ui/CampaignMapEditor.h"
#include "ui/CampaignMapWatcher.h"
#include "ui/MapUtil.h"
#include "utils/Constants.h"
#include "utils/Logging.h"

namespace warmonger {
namespace ui {

CampaignMapEditor::CampaignMapEditor(QQuickItem *parent) :
    BasicMap(parent),
    campaignMap(nullptr),
    worldSurface(nullptr),
    currentMapNode(nullptr),
    editingMode(EditingMode::TerrainType),
    objectType(nullptr),
    watcher(nullptr)
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

        delete this->watcher;
        this->watcher = new CampaignMapWatcher(this->campaignMap, this);
        QObject::connect(this->watcher, &CampaignMapWatcher::changed, this, &CampaignMapEditor::update);

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

CampaignMapEditor::EditingMode CampaignMapEditor::getEditingMode() const
{
    return this->editingMode;
}

void CampaignMapEditor::setEditingMode(EditingMode editingMode)
{
    if (this->editingMode != editingMode)
    {
        this->editingMode = editingMode;
        emit editingModeChanged();
    }
}

QObject* CampaignMapEditor::getObjectType() const
{
    return this->objectType;
}

void CampaignMapEditor::setObjectType(QObject* objectType)
{
    if (this->objectType != objectType)
    {
        wDebug << "objectType: `" << this->objectType << "' -> `" << objectType << "'";
        this->objectType = objectType;
        emit objectTypeChanged();
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
            this->windowPosToMapPos(event->pos()),
            this->campaignMap->getMapNodes(),
            this->mapNodesPos,
            this->worldSurface);

    if (this->currentMapNode != nextCurrentMapNode)
    {
        this->currentMapNode = nextCurrentMapNode;
        this->update();
    }
}

void CampaignMapEditor::hoverLeaveEvent(QHoverEvent*)
{
    this->currentMapNode = nullptr;
    this->update();
}

void CampaignMapEditor::mousePressEvent(QMouseEvent* event)
{
    this->doEditingAction(this->windowPosToMapPos(event->pos()));

    BasicMap::mousePressEvent(event);
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

void CampaignMapEditor::doEditingAction(const QPoint& pos)
{
    switch(this->editingMode)
    {
        case EditingMode::TerrainType:
            this->doTerrainTypeEditingAction(pos);
            break;
        case EditingMode::SettlementType:
            this->doSettlementTypeEditingAction();
            break;
    }
}

void CampaignMapEditor::doTerrainTypeEditingAction(const QPoint&)
{
    core::TerrainType* terrainType = qobject_cast<core::TerrainType*>(this->objectType);
    if(terrainType == nullptr)
    {
        wWarning << "objectType has invalid value `" << this->objectType << "' for editing mode `EditingMode::TerrainType'";
        return;
    }

    core::MapNode* mapNode{nullptr};

    if(this->currentMapNode != nullptr)
    {
        mapNode = this->currentMapNode;
    }
    else
    {
        //TODO:
    }

    if(mapNode != nullptr)
    {
        mapNode->setTerrainType(terrainType);
    }
}

void CampaignMapEditor::doSettlementTypeEditingAction()
{
}

} // namespace ui
} // namespace warmonger
