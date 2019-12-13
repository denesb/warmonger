/**
 * Copyright (C) 2015-2018 Botond Dénes
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

#include "ui/MapEditor.h"
#include "ui/MapUtil.h"
#include "ui/MapWatcher.h"
#include "utils/Constants.h"
#include "utils/Logging.h"
#include "utils/QVariantUtils.h"

namespace warmonger {
namespace ui {

MapEditor::MapEditor(QQuickItem* parent)
    : BasicMap(parent)
    , map(nullptr)
    , worldSurface(nullptr)
    , hoverMapNode(nullptr)
    , editingMode(EditingMode::None)
    , currentFaction(nullptr)
    , watcher(nullptr)
{
    this->setAcceptHoverEvents(true);
}

void MapEditor::setMap(core::Map* map)
{
    if (this->map != map)
    {
        wInfo << "map `" << this->map << "' -> `" << map << "'";

        if (this->map != nullptr)
        {
            QObject::disconnect(this->map, nullptr, this, nullptr);
            delete this->watcher;
        }

        this->map = map;
        this->updateContent();

        if (this->map)
        {
            this->watcher = new MapWatcher(this->map, this);
            QObject::connect(this->watcher, &MapWatcher::changed, this, &MapEditor::update);
            QObject::connect(this->map, &core::Map::mapNodesChanged, this, &MapEditor::onMapNodesChanged);
        }

        emit mapChanged();
    }
}

void MapEditor::setWorldSurface(WorldSurface* worldSurface)
{
    if (this->worldSurface != worldSurface)
    {
        wInfo << "worldSurface `" << this->worldSurface << "' -> `" << worldSurface << "'";

        this->worldSurface = worldSurface;
        this->updateContent();

        emit worldSurfaceChanged();
    }
}

void MapEditor::setEditingMode(EditingMode editingMode)
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

QSGNode* MapEditor::updatePaintNode(QSGNode* oldRootNode, UpdatePaintNodeData*)
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

void MapEditor::setNumberOfFactions(int n)
{
    if (this->map == nullptr)
        return;

    std::size_t newSize = static_cast<std::size_t>(n);
    std::size_t currentSize = this->map->getFactions().size();

    if (newSize > currentSize)
    {
        const std::vector<core::Civilization*>& civilizations = this->map->getWorld()->getCivilizations();

        std::random_device rd;
        std::mt19937 mtd(rd());
        std::uniform_int_distribution<std::size_t> dist(0, civilizations.size() - 1);

        for (std::size_t i = currentSize; i < newSize; ++i)
        {
            auto faction = this->map->createFaction();
            faction->setCivilization(civilizations.at(dist(mtd)));
        }
    }
    else if (newSize < currentSize)
    {
        const std::vector<core::Faction*>& factions = this->map->getFactions();
        for (std::size_t i = newSize; i < currentSize; ++i)
        {
            this->map->removeFaction(factions.at(i));
        }
    }
}

void MapEditor::setCurrentFaction(core::Faction* currentFaction)
{
    if (this->currentFaction != currentFaction)
    {
        wDebug << "currentFaction: `" << this->currentFaction << "' -> `" << currentFaction << "'";

        this->currentFaction = currentFaction;

        emit currentFactionChanged();
    }
}

void MapEditor::hoverMoveEvent(QHoverEvent* event)
{
    const QPoint mapPos = this->windowPosToMapPos(event->pos());
    core::MapNode* currentMapNode = mapNodeAtPos(mapPos, this->mapNodesPos, this->worldSurface);

    std::experimental::optional<QPoint> currentHoverPos;

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
            currentHoverPos = std::experimental::nullopt;
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

void MapEditor::hoverEnterEvent(QHoverEvent*)
{
    QGuiApplication::setOverrideCursor(QCursor(Qt::ArrowCursor));
}

void MapEditor::hoverLeaveEvent(QHoverEvent*)
{
    this->hoverMapNode = nullptr;
    this->hoverPos = std::experimental::nullopt;

    QGuiApplication::restoreOverrideCursor();

    this->update();
}

void MapEditor::mousePressEvent(QMouseEvent* event)
{
    this->doEditingAction(this->windowPosToMapPos(event->pos()));

    BasicMap::mousePressEvent(event);
}

void MapEditor::updateContent()
{
    if (this->worldSurface == nullptr || this->map == nullptr || this->map->getMapNodes().empty() ||
        this->worldSurface->getWorld() != this->map->getWorld())
    {
        this->setFlags(0);
    }
    else
    {
        this->setFlags(QQuickItem::ItemHasContents);
        this->mapNodesPos = positionMapNodes(this->map->getMapNodes()[0], this->worldSurface->getTileSize());
        this->updateMapRect();
    }
}

void MapEditor::updateMapRect()
{
    if (this->worldSurface == nullptr || this->map == nullptr || this->map->getMapNodes().empty() ||
        this->worldSurface->getWorld() != this->map->getWorld())
    {
        this->setMapRect(QRect(0, 0, 0, 0));
    }
    else
    {
        this->setMapRect(calculateBoundingRect(this->mapNodesPos, this->worldSurface->getTileSize()));
        this->update();
    }
}

void MapEditor::onMapNodesChanged()
{
    this->mapNodesPos = positionMapNodes(this->map->getMapNodes()[0], this->worldSurface->getTileSize());
    this->updateMapRect();
}

void MapEditor::doEditingAction(const QPoint&)
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

void MapEditor::doGrantToCurrentFactionEditingAction()
{
}

bool MapEditor::isCurrentEditingActionPossible() const
{
    return true;
}

} // namespace ui
} // namespace warmonger
