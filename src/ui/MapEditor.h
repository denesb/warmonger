/** \file
 * MapEditor class.
 *
 * \copyright (C) 2015-2018 Botond Dénes
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

#ifndef W_UI_MAP_EDITOR_H
#define W_UI_MAP_EDITOR_H

#include <experimental/optional>
#include <unordered_map>

#include "core/Map.h"
#include "ui/BasicMap.h"
#include "ui/WorldSurface.h"

namespace warmonger {
namespace ui {

class MapWatcher;

/**
 * Presents an editing widget for a campaign-map.
 *
 * For the map to be actually drawn the MapEditor needs a campaign-map
 * and a matching, actvivated  world-surface (that belongs to the same world)
 * set. When these conditions are not met the QQuickItem::ItemHasContents flag
 * is unset and nothing is going to be drawn on the screen.
 *
 * \see MapEditor::setMap
 * \see MapEditor::setWorldSurface
 * \see core::Map
 * \see WorldSurface
 */
class MapEditor : public BasicMap
{
    Q_OBJECT
    Q_PROPERTY(warmonger::core::Map* map READ getMap WRITE setMap NOTIFY mapChanged)
    Q_PROPERTY(WorldSurface* worldSurface READ getWorldSurface WRITE setWorldSurface NOTIFY worldSurfaceChanged)
    Q_PROPERTY(warmonger::ui::MapEditor::EditingMode editingMode READ getEditingMode WRITE setEditingMode NOTIFY
            editingModeChanged)
    Q_PROPERTY(warmonger::core::Faction* currentFaction READ getCurrentFaction WRITE setCurrentFaction NOTIFY
            currentFactionChanged)

public:
    /**
     * Editing modes.
     *
     * \see MapEditor::setEditingMode()
     */
    enum class EditingMode
    {
        None,
        Edit,
        Remove,
        GrantToCurrentFaction
    };
    Q_ENUM(EditingMode)

    /**
     * Constructs an empty map-editor.
     */
    MapEditor(QQuickItem* parent = nullptr);

    /**
     * Get the edited campaign-map
     *
     * The MapEditor does not own the campaign-map!
     *
     * \return the campaign-map
     */
    core::Map* getMap() const
    {
        return this->map;
    }

    /**
     * Set the campaign map to edit.
     *
     * If all conditions are given for drawing the map, this will trigger a
     * redraw. If this was the missing piece it will trigger the first drawing.
     * The MapEditor does not assume ownership of the campaign-map!
     * Will emit the signal MapEditor::mapChanged() if the newly
     * set value is different than the current one.
     *
     * \param map the campaign-map
     */
    void setMap(core::Map* map);

    /**
     * Get the world-surface used for drawing the campign-map.
     *
     * The MapEditor does not own the world-surface!
     *
     * \return the world-surface
     */
    WorldSurface* getWorldSurface() const
    {
        return this->worldSurface;
    }

    /**
     * Set the world-surface used for drawing the map.
     *
     * If all conditions are given for drawing the map, this will trigger a
     * redraw. If this was the missing piece it will trigger the first drawing.
     * The MapEditor does not assume ownership of the world-surface!
     * Will emit the signal MapEditor::worldSurfaceChanged() if the
     * newly set value is different than the current one.
     *
     * \param worldSurface the world-surface
     */
    void setWorldSurface(WorldSurface* worldSurface);

    /**
     * Get the editing-mode.
     *
     * \return the editing-mode
     *
     * \see MapEditor::setEditingMode()
     */
    EditingMode getEditingMode() const
    {
        return this->editingMode;
    }

    /**
     * Set the editing-mode.
     *
     * The editing-mode determines what happens when the
     * user clicks on the map-editor.
     * Will emit the signal MapEditor::editingModeChanged() if the
     * newly set value is different than the current one.
     *
     * \param editingMode the editing-mode
     */
    void setEditingMode(EditingMode editingMode);

    /**
     * Update the scene-graph.
     *
     * \see QQuickItem::updatePaintNode()
     */
    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* oldNodeData) override;

    /**
     * Set the number of factions on the map.
     *
     * Factions will be created or removed to match the set number.
     * If there is no campaign-map set this method will have no effect.
     *
     * \param n the new number of factions
     */
    Q_INVOKABLE void setNumberOfFactions(int n);

    /**
     * Get the current faction.
     *
     * \return the current faction
     *
     * \see setCurrentFaction()
     */
    core::Faction* getCurrentFaction() const
    {
        return this->currentFaction;
    }

    /**
     * Set the current faction.
     *
     * The current faction determines the owner of newly created settlements
     * and armies or the clicked-on ones when the editing-mode is
     * EditingMode::GrantToCurrentFaction.
     * The current faction can be nullptr (unassigned), in this case the
     * settlements and armies will have no owner.
     * Will emit the signal MapEditor::currentFactionChanged() if the
     * newly set value is different than the current one.
     *
     * \param currentFaction the current faction
     */
    void setCurrentFaction(core::Faction* currentFaction);

signals:
    /**
     * Emitted when the map-node changes.
     */
    void mapChanged();

    /**
     * Emitted when the world-surface changes.
     */
    void worldSurfaceChanged();

    /**
     * Emitted when the editing-mode changes.
     */
    void editingModeChanged();

    /**
     * Emitted when the current faction changes.
     */
    void currentFactionChanged();

protected:
    void hoverMoveEvent(QHoverEvent* event) override;
    void hoverEnterEvent(QHoverEvent* event) override;
    void hoverLeaveEvent(QHoverEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    void updateContent();
    void updateMapRect();
    void onMapNodesChanged();
    void doEditingAction(const QPoint& pos);
    void doGrantToCurrentFactionEditingAction();
    bool isCurrentEditingActionPossible() const;

    core::Map* map;
    WorldSurface* worldSurface;
    std::unordered_map<core::MapNode*, QPoint> mapNodesPos;

    core::MapNode* hoverMapNode;
    std::experimental::optional<QPoint> hoverPos;

    EditingMode editingMode;
    core::Faction* currentFaction;

    MapWatcher* watcher;
};

} // namespace ui
} // namespace warmonger

#endif // W_UI_MAP_EDITOR_H
