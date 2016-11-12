#include <random>

#include <QCursor>
#include <QGuiApplication>
#include <QMetaEnum>
#include <QSGSimpleTextureNode>

#include "core/Utils.hpp"
#include "ui/CampaignMapEditor.h"
#include "ui/CampaignMapWatcher.h"
#include "ui/MapUtil.h"
#include "utils/Constants.h"
#include "utils/Logging.h"
#include "utils/QVariantUtils.h"

namespace warmonger {
namespace ui {

CampaignMapEditor::CampaignMapEditor(QQuickItem* parent)
    : BasicMap(parent)
    , campaignMap(nullptr)
    , worldSurface(nullptr)
    , hoverMapNode(nullptr)
    , editingMode(EditingMode::None)
    , objectType(nullptr)
    , currentFaction(nullptr)
    , watcher(nullptr)
{
    this->setAcceptHoverEvents(true);

    QObject::connect(
        this, &CampaignMapEditor::editingModeChanged, this, &CampaignMapEditor::availableObjectTypesChanged);
    QObject::connect(
        this, &CampaignMapEditor::campaignMapChanged, this, &CampaignMapEditor::availableObjectTypesChanged);
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
                this->campaignMap, &core::CampaignMap::mapNodesChanged, this, &CampaignMapEditor::onMapNodesChanged);
        }

        emit campaignMapChanged();
    }
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

void CampaignMapEditor::setEditingMode(EditingMode editingMode)
{
    if (this->editingMode != editingMode)
    {
        QMetaEnum me = QMetaEnum::fromType<EditingMode>();
        wDebug << "editingMode: `" << me.valueToKey(static_cast<int>(this->editingMode)) << "' -> `"
               << me.valueToKey(static_cast<int>(editingMode)) << "'";

        this->editingMode = editingMode;
        emit editingModeChanged();
    }
}

QVariantList CampaignMapEditor::readAvailableObjectTypes() const
{
    if (this->campaignMap == nullptr)
        return QVariantList();

    const core::World* world = this->campaignMap->getWorld();

    switch (this->editingMode)
    {
        case EditingMode::TerrainType:
            return world->readTerrainTypes();

        case EditingMode::SettlementType:
            return world->readSettlementTypes();

        case EditingMode::ArmyType:
            return world->readArmyTypes();

        default:
            return QVariantList();
    }

    return QVariantList();
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

QSGNode* CampaignMapEditor::updatePaintNode(QSGNode* oldRootNode, UpdatePaintNodeData*)
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

    const std::vector<core::CampaignMap::Content> contents = visibleContents(
        this->campaignMap->getContents(), this->mapNodesPos, this->worldSurface->getTileSize(), this->getWindowRect());

    drawContents(contents, mapRootNode, *this);

    return rootNode;
}

QSGNode* CampaignMapEditor::drawContent(const core::CampaignMap::Content& content, QSGNode* oldNode)
{
    core::MapNode* mapNode = std::get<core::MapNode*>(content);
    const QPoint& pos = this->mapNodesPos.at(mapNode);

    QSGNode* mapSGNode = drawMapNode(mapNode, this->worldSurface, this->window(), pos, oldNode);

    if (std::get<core::Settlement*>(content) != nullptr)
    {
        QSGNode* settlementSGNode = drawSettlement(
            std::get<core::Settlement*>(content), this->worldSurface, this->window(), pos, mapSGNode->firstChild());

        if (mapSGNode->firstChild() == nullptr)
            mapSGNode->appendChildNode(settlementSGNode);
    }
    else if (std::get<core::Army*>(content) != nullptr)
    {
        QSGNode* armySGNode =
            drawArmy(std::get<core::Army*>(content), this->worldSurface, this->window(), pos, mapSGNode->firstChild());

        if (mapSGNode->firstChild() == nullptr)
            mapSGNode->appendChildNode(armySGNode);
    }

    return mapSGNode;
}

void CampaignMapEditor::setNumberOfFactions(int n)
{
    if (this->campaignMap == nullptr)
        return;

    std::size_t newSize = static_cast<std::size_t>(n);
    std::size_t currentSize = this->campaignMap->getFactions().size();

    if (newSize > currentSize)
    {
        const std::vector<core::Civilization*>& civilizations = this->campaignMap->getWorld()->getCivilizations();

        std::random_device rd;
        std::mt19937 mtd(rd());
        std::uniform_int_distribution<std::size_t> dist(0, civilizations.size() - 1);

        for (std::size_t i = currentSize; i < newSize; ++i)
        {
            this->campaignMap->createFaction(civilizations.at(dist(mtd)));
        }
    }
    else if (newSize < currentSize)
    {
        const std::vector<core::Faction*>& factions = this->campaignMap->getFactions();
        for (std::size_t i = newSize; i < currentSize; ++i)
        {
            this->campaignMap->removeFaction(factions.at(i));
        }
    }
}

void CampaignMapEditor::setCurrentFaction(core::Faction* currentFaction)
{
    if (this->currentFaction != currentFaction)
    {
        wDebug << "currentFaction: `" << this->currentFaction << "' -> `" << currentFaction << "'";

        this->currentFaction = currentFaction;

        emit currentFactionChanged();
    }
}

void CampaignMapEditor::hoverMoveEvent(QHoverEvent* event)
{
    const QPoint mapPos = this->windowPosToMapPos(event->pos());
    core::MapNode* currentMapNode = mapNodeAtPos(mapPos, this->mapNodesPos, this->worldSurface);

    boost::optional<QPoint> currentHoverPos;

    if (currentMapNode == nullptr)
    {
        const core::MapNodeNeighbours neighbours = neighboursByPos(mapPos, this->worldSurface, this->mapNodesPos);
        const auto it = std::find_if(neighbours.cbegin(),
            neighbours.cend(),
            [](const std::pair<core::Direction, core::MapNode*>& i) { return i.second != nullptr; });

        if (it != neighbours.cend())
        {
            currentHoverPos = neighbourPos(
                this->mapNodesPos[it->second], core::oppositeDirection(it->first), this->worldSurface->getTileSize());
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

        if (this->isCurrentEditingActionPossible())
            QGuiApplication::changeOverrideCursor(QCursor(Qt::ArrowCursor));
        else
            QGuiApplication::changeOverrideCursor(QCursor(Qt::ForbiddenCursor));

        this->update();
    }
}

void CampaignMapEditor::hoverEnterEvent(QHoverEvent*)
{
    QGuiApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
}

void CampaignMapEditor::hoverLeaveEvent(QHoverEvent*)
{
    this->hoverMapNode = nullptr;
    this->hoverPos = boost::none;

    QGuiApplication::restoreOverrideCursor();

    this->update();
}

void CampaignMapEditor::mousePressEvent(QMouseEvent* event)
{
    this->doEditingAction(this->windowPosToMapPos(event->pos()));

    BasicMap::mousePressEvent(event);
}

void CampaignMapEditor::updateContent()
{
    if (this->worldSurface == nullptr || this->campaignMap == nullptr || this->campaignMap->getMapNodes().empty() ||
        this->worldSurface->getWorld() != this->campaignMap->getWorld())
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
    if (this->worldSurface == nullptr || this->campaignMap == nullptr || this->campaignMap->getMapNodes().empty() ||
        this->worldSurface->getWorld() != this->campaignMap->getWorld())
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
    switch (this->editingMode)
    {
        case EditingMode::TerrainType:
            this->doTerrainTypeEditingAction(pos);
            break;

        case EditingMode::SettlementType:
            this->doSettlementTypeEditingAction();
            break;

        case EditingMode::ArmyType:
            this->doArmyTypeEditingAction();
            break;

        case EditingMode::Edit:
            // this->doEditEditingAction();
            break;

        case EditingMode::Remove:
            // this->doRemoveTypeEditingAction();
            break;

        case EditingMode::GrantToCurrentFaction:
            this->doGrantToCurrentFactionEditingAction();
            break;

        case EditingMode::None:
            break;
    }
}

void CampaignMapEditor::doTerrainTypeEditingAction(const QPoint& pos)
{
    core::TerrainType* terrainType = qobject_cast<core::TerrainType*>(this->objectType);
    if (terrainType == nullptr)
    {
        wWarning << "objectType has invalid value `" << this->objectType
                 << "' for editing mode `EditingMode::TerrainType'";
        return;
    }

    if (this->hoverMapNode == nullptr)
    {
        if (this->hoverPos)
        {
            const core::MapNodeNeighbours neighbours(neighboursByPos(pos, this->worldSurface, this->mapNodesPos));
            if (!neighbours.empty())
            {
                this->hoverMapNode = this->campaignMap->createMapNode(terrainType, neighbours);

                wDebug << "Creating new node " << this->hoverMapNode;
            }
        }
    }
    else
    {
        this->hoverMapNode->setTerrainType(terrainType);

        wDebug << "Editing existing node " << this->hoverMapNode;
    }
}

void CampaignMapEditor::doSettlementTypeEditingAction()
{
    core::SettlementType* settlementType = qobject_cast<core::SettlementType*>(this->objectType);
    if (settlementType == nullptr)
    {
        wWarning << "objectType has invalid value `" << this->objectType
                 << "' for editing mode `EditingMode::SettlementType'";
        return;
    }

    core::MapNode* currentMapNode = this->hoverMapNode;

    if (currentMapNode == nullptr)
        return;

    const std::vector<core::CampaignMap::Content>& contents = this->campaignMap->getContents();

    auto it = std::find_if(contents.begin(), contents.end(), core::HasMapNode(currentMapNode));

    if (it == contents.end())
        return;

    if (std::get<core::Army*>(*it) != nullptr || std::get<core::Settlement*>(*it) != nullptr)
        return;

    core::Settlement* settlement = this->campaignMap->createSettlement(settlementType);

    settlement->setMapNode(currentMapNode);
    settlement->setOwner(currentFaction);

    wDebug << "Created new settlement " << settlement;
}

void CampaignMapEditor::doArmyTypeEditingAction()
{
    core::ArmyType* armyType = qobject_cast<core::ArmyType*>(this->objectType);
    if (armyType == nullptr)
    {
        wWarning << "objectType has invalid value `" << this->objectType
                 << "' for editing mode `EditingMode::ArmyType'";
        return;
    }

    core::MapNode* currentMapNode = this->hoverMapNode;

    if (currentMapNode == nullptr)
        return;

    const std::vector<core::CampaignMap::Content>& contents = this->campaignMap->getContents();

    auto it = std::find_if(contents.begin(), contents.end(), core::HasMapNode(currentMapNode));

    if (it == contents.end())
    {
        wWarning << "hoverMapNode not in campaign-map's contents";
        return;
    }

    if (std::get<core::Army*>(*it) != nullptr || std::get<core::Settlement*>(*it) != nullptr)
        return;

    core::Army* army = this->campaignMap->createArmy(armyType);

    army->setMapNode(currentMapNode);
    army->setOwner(currentFaction);

    wDebug << "Created new army " << army;
}

void CampaignMapEditor::doGrantToCurrentFactionEditingAction()
{
    core::MapNode* currentMapNode = this->hoverMapNode;

    const std::vector<core::CampaignMap::Content>& contents = this->campaignMap->getContents();

    auto it = std::find_if(contents.begin(), contents.end(), core::HasMapNode(currentMapNode));

    if (it == contents.end())
    {
        wWarning << "hoverMapNode not in campaign-map's contents";
        return;
    }

    core::Army* army = std::get<core::Army*>(*it);
    if (army != nullptr)
    {
        army->setOwner(this->currentFaction);

        wDebug << "Granted " << army << " to " << this->currentFaction;

        return;
    }

    core::Settlement* settlement = std::get<core::Settlement*>(*it);
    if (settlement != nullptr)
    {
        settlement->setOwner(this->currentFaction);

        wDebug << "Granted " << settlement << " to " << this->currentFaction;

        return;
    }
}

bool CampaignMapEditor::isCurrentEditingActionPossible() const
{
    boost::optional<core::CampaignMap::Content> content;

    if (this->hoverMapNode != nullptr)
    {
        const std::vector<core::CampaignMap::Content>& contents = this->campaignMap->getContents();

        auto it = std::find_if(contents.begin(), contents.end(), core::HasMapNode(this->hoverMapNode));
        content = *it;
    }

    switch (this->editingMode)
    {
        case EditingMode::TerrainType:
            return true;

        case EditingMode::SettlementType:
        case EditingMode::ArmyType:
            return content && std::get<core::Settlement*>(*content) == nullptr &&
                std::get<core::Army*>(*content) == nullptr;

        case EditingMode::Edit:
        case EditingMode::Remove:
        case EditingMode::GrantToCurrentFaction:
            return content &&
                (std::get<core::Settlement*>(*content) != nullptr || std::get<core::Army*>(*content) != nullptr);

        case EditingMode::None:
            return true;
    }

    return true;
}

} // namespace ui
} // namespace warmonger
