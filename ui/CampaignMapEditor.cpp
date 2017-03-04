/**
 * Copyright (C) 2015-2017 Botond Dénes
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <random>

#include <QCursor>
#include <QGuiApplication>
#include <QMetaEnum>
#include <QSGSimpleTextureNode>

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
    , currentFaction(nullptr)
    , watcher(nullptr)
{
    this->setAcceptHoverEvents(true);
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

    // TODO: invoke GraphicsSystem

    return rootNode;
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

void CampaignMapEditor::doEditingAction(const QPoint&)
{
    switch (this->editingMode)
    {
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

void CampaignMapEditor::doGrantToCurrentFactionEditingAction()
{
}

bool CampaignMapEditor::isCurrentEditingActionPossible() const
{
    return true;
}

} // namespace ui
} // namespace warmonger
