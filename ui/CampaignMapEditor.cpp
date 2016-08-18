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
    hoverMapNode(nullptr),
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

        if (this->campaignMap != nullptr)
        {
            QObject::disconnect(this->campaignMap, nullptr, this, nullptr);
            delete this->watcher;
        }

        this->campaignMap = campaignMap;
        this->updateContent();

        if (this->campaignMap)
        {
            this->watcher = new CampaignMapWatcher(this->campaignMap, this);
            QObject::connect(this->watcher, &CampaignMapWatcher::changed, this, &CampaignMapEditor::update);
            QObject::connect(
                    this->campaignMap,
                    &core::CampaignMap::mapNodesChanged,
                    this,
                    &CampaignMapEditor::onMapNodesChanged);
        }

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
    QSGTransformNode* transformNode;
    QSGNode* mapRootNode;
    if (oldRootNode == nullptr)
    {
        rootNode = new QSGClipNode();
        rootNode->setIsRectangular(true);

        transformNode = new QSGTransformNode();
        transformNode->setMatrix(transform);
        rootNode->appendChildNode(transformNode);

        mapRootNode = new QSGNode();
        transformNode->appendChildNode(mapRootNode);
    }
    else
    {
        rootNode = static_cast<QSGClipNode*>(oldRootNode);

        transformNode = static_cast<QSGTransformNode*>(rootNode->firstChild());
        if (transform != transformNode->matrix())
        {
            transformNode->setMatrix(transform);
        }

        mapRootNode = transformNode->firstChild();
    }

    rootNode->setClipRect(QRectF(0, 0, this->width(), this->height()));

    const std::vector<core::MapNode*> mapNodes = visibleMapNodes(
            this->mapNodesPos,
            this->worldSurface->getTileSize(),
            this->getWindowRect());

    drawMapNodes(mapNodes, mapRootNode, *this);

    QSGNode* oldHoverNode{nullptr};
    if (transformNode->childCount() == 2)
        oldHoverNode = transformNode->lastChild();

    QSGNode* hoverNode = this->drawHoverNode(oldHoverNode);

    if (hoverNode == nullptr)
    {
        if (oldHoverNode != nullptr)
            transformNode->removeChildNode(oldHoverNode);
    }
    else
    {
        if (oldHoverNode == nullptr)
            transformNode->appendChildNode(hoverNode);
    }

    return rootNode;
}

QSGNode* CampaignMapEditor::drawMapNodeAndContents(core::MapNode* mapNode, QSGNode* oldNode)
{
    return drawMapNode(mapNode, this->worldSurface, this->mapNodesPos.at(mapNode), oldNode);
}

void CampaignMapEditor::hoverMoveEvent(QHoverEvent* event)
{
    const QPoint mapPos = this->windowPosToMapPos(event->pos());
    core::MapNode* currentMapNode = mapNodeAtPos(mapPos, this->mapNodesPos, this->worldSurface);

    boost::optional<QPoint> currentHoverPos;

    if(currentMapNode == nullptr)
    {
        const core::MapNodeNeighbours neighbours = neighboursByPos(mapPos, this->worldSurface, this->mapNodesPos);
        const auto it = std::find_if(
                neighbours.cbegin(),
                neighbours.cend(),
                [](const std::pair<utils::Direction, core::MapNode*>& i){ return i.second != nullptr; });

        if (it != neighbours.cend())
        {
            currentHoverPos = neighbourPos(
                    this->mapNodesPos[it->second],
                    utils::oppositeDirection(it->first),
                    this->worldSurface->getTileSize());
        }
        else
        {
            currentHoverPos = boost::none;
        }
    }
    else
    {
        currentHoverPos = this->mapNodesPos[currentMapNode];
    }

    if (this->hoverPos != currentHoverPos)
    {
        this->hoverMapNode = currentMapNode;
        this->hoverPos = currentHoverPos;
        this->update();
    }
}

void CampaignMapEditor::hoverLeaveEvent(QHoverEvent*)
{
    this->hoverMapNode = nullptr;
    this->hoverPos = boost::none;
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

void CampaignMapEditor::onMapNodesChanged()
{
    this->mapNodesPos = positionMapNodes(this->campaignMap->getMapNodes()[0], this->worldSurface->getTileSize());
    this->updateMapRect();
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

void CampaignMapEditor::doTerrainTypeEditingAction(const QPoint& pos)
{
    core::TerrainType* terrainType = qobject_cast<core::TerrainType*>(this->objectType);
    if(terrainType == nullptr)
    {
        wWarning << "objectType has invalid value `" << this->objectType
            << "' for editing mode `EditingMode::TerrainType'";
        return;
    }

    core::MapNode* mapNode{nullptr};

    if(this->hoverMapNode == nullptr)
    {
        if (this->hoverPos)
        {
            const core::MapNodeNeighbours neighbours(neighboursByPos(pos, this->worldSurface, this->mapNodesPos));
            if(!neighbours.empty())
            {
                mapNode = new core::MapNode();
                mapNode->setObjectName("mapNode" + QString::number(this->mapNodesPos.size()));
                mapNode->setNeighbours(neighbours);

                for (const std::pair<utils::Direction, core::MapNode*>& neighbour : neighbours)
                    if (neighbour.second != nullptr)
                        neighbour.second->setNeighbour(utils::oppositeDirection(neighbour.first), mapNode);

                this->campaignMap->addMapNode(mapNode);

                this->hoverMapNode = mapNode;

                wDebug << "Creating new node " << mapNode;
            }
        }
    }
    else
    {
        mapNode = this->hoverMapNode;

        wDebug << "Editing existing node " << mapNode;
    }

    if(mapNode != nullptr)
    {
        mapNode->setTerrainType(terrainType);
    }
}

void CampaignMapEditor::doSettlementTypeEditingAction()
{
}

QSGNode* CampaignMapEditor::drawHoverNode(QSGNode* oldNode) const
{
    if (!this->hoverPos)
        return nullptr;

    QSGSimpleTextureNode* hoverNode;
    if(oldNode == nullptr)
        hoverNode = new QSGSimpleTextureNode();
    else
        hoverNode = static_cast<QSGSimpleTextureNode*>(oldNode);

    QSGTexture* texture = this->worldSurface->getTexture(utils::resourcePaths::mapEditor::hoverValid);
    if (texture != hoverNode->texture())
        hoverNode->setTexture(texture);

    const QRect hoverRect(*this->hoverPos, this->worldSurface->getTileSize());
    if (hoverNode->rect() != hoverRect)
        hoverNode->setRect(hoverRect);

    return hoverNode;
}

} // namespace ui
} // namespace warmonger
