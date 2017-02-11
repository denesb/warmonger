/** \file
 * CampaignMapEditor class.
 *
 * \copyright (C) 2015-2017 Botond Dénes
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

#ifndef W_UI_CAMPAIGN_MAP_EDITOR_H
#define W_UI_CAMPAIGN_MAP_EDITOR_H

#include <map>

#include <boost/optional.hpp>

#include "core/CampaignMap.h"
#include "ui/BasicMap.h"
#include "ui/CampaignMapDrawer.h"
#include "ui/WorldSurface.h"

namespace warmonger {
namespace ui {

class CampaignMapWatcher;

/**
 * Presents an editing widget for a campaign-map.
 *
 * For the map to be actually drawn the CampaignMapEditor needs a campaign-map
 * and a matching, actvivated  world-surface (that belongs to the same world)
 * set. When these conditions are not met the QQuickItem::ItemHasContents flag
 * is unset and nothing is going to be drawn on the screen.
 *
 * \see CampaignMapEditor::setCampaignMap
 * \see CampaignMapEditor::setWorldSurface
 * \see core::CampaignMap
 * \see WorldSurface
 */
class CampaignMapEditor : public BasicMap, public CampaignMapDrawer
{
    Q_OBJECT
    Q_PROPERTY(
        warmonger::core::CampaignMap* campaignMap READ getCampaignMap WRITE setCampaignMap NOTIFY campaignMapChanged)
    Q_PROPERTY(WorldSurface* worldSurface READ getWorldSurface WRITE setWorldSurface NOTIFY worldSurfaceChanged)
    Q_PROPERTY(warmonger::ui::CampaignMapEditor::EditingMode editingMode READ getEditingMode WRITE setEditingMode NOTIFY
            editingModeChanged)
    Q_PROPERTY(QVariantList availableObjectTypes READ readAvailableObjectTypes NOTIFY availableObjectTypesChanged)
    Q_PROPERTY(QObject* objectType READ getObjectType WRITE setObjectType NOTIFY objectTypeChanged)
    Q_PROPERTY(warmonger::core::Faction* currentFaction READ getCurrentFaction WRITE setCurrentFaction NOTIFY
            currentFactionChanged)

public:
    /**
     * Editing modes.
     *
     * \see CampaignMapEditor::setEditingMode()
     */
    enum class EditingMode
    {
        None,
        TerrainType,
        SettlementType,
        ArmyType,
        Edit,
        Remove,
        GrantToCurrentFaction
    };
    Q_ENUM(EditingMode)

    /**
     * Constructs an empty map-editor.
     */
    CampaignMapEditor(QQuickItem* parent = nullptr);

    /**
     * Get the edited campaign-map
     *
     * The CampaignMapEditor does not own the campaign-map!
     *
     * \return the campaign-map
     */
    core::CampaignMap* getCampaignMap() const
    {
        return this->campaignMap;
    }

    /**
     * Set the campaign map to edit.
     *
     * If all conditions are given for drawing the map, this will trigger a
     * redraw. If this was the missing piece it will trigger the first drawing.
     * The CampaignMapEditor does not assume ownership of the campaign-map!
     * Will emit the signal CampaignMapEditor::campaignMapChanged() if the newly
     * set value is different than the current one.
     *
     * \param campaignMap the campaign-map
     */
    void setCampaignMap(core::CampaignMap* campaignMap);

    /**
     * Get the world-surface used for drawing the campign-map.
     *
     * The CampaignMapEditor does not own the world-surface!
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
     * The CampaignMapEditor does not assume ownership of the world-surface!
     * Will emit the signal CampaignMapEditor::worldSurfaceChanged() if the
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
     * \see CampaignMapEditor::setEditingMode()
     */
    EditingMode getEditingMode() const
    {
        return this->editingMode;
    }

    /**
     * Set the editing-mode.
     *
     * The editing-mode together with the object-type (\see
     * CampaignMapEditor::setObjectType()) determines what happens when the
     * user clicks on the map-editor. Editing-mode determines the class of
     * objects that the will be editited. E.g. the TerrainType editing-mode
     * means that all editing-actions will target the map-nodes.
     * Will emit the signal CampaignMapEditor::editingModeChanged() if the
     * newly set value is different than the current one.
     *
     * \param editingMode - the editing-mode
     */
    void setEditingMode(EditingMode editingMode);

    /**
     * Read method which returns the available object-types.
     *
     * The available object-types depend on the current editing-mode. Some
     * editing-modes might have no available object-types, in this case the
     * return list will be empty.
     * This function is used as a read function for the availableObjectTypes
     * property and is not supposed to be called from C++ code.
     *
     * \returns the available object-types as a QVariantList
     *
     * \see CampaignMapEditor::setObjectType()
     * \see CampaignMapEditor::setEditingMode()
     */
    QVariantList readAvailableObjectTypes() const;

    /**
     * Get the object-type.
     *
     * \returns the object-type
     *
     * \see CampaignMapEditor::setObjectType()
     */
    QObject* getObjectType() const
    {
        return this->objectType;
    }

    /**
     * Set the object-type.
     *
     * The object-type together with the editing-mode determines what happens
     * when the user clicks on the map. While the editing-mode determines the
     * kind-of object that will be affected the object-type determines what
     * will the affected object become, e.g. if the editing-mode is
     * EditingMode::TerrainType and the objectType is *plains* then when the
     * user clicks on a map-node its terrain-type is going to be changed to
     * *plains*.
     * The object-type has to be one of the available object-types as returned
     * by CampaignMapEditor::readAvailableObjectTypes().
     * Will emit the signal CampaignMapEditor::objectTypeChanged() if the
     * newly set value is different than the current one.
     *
     * \param objectType - the object-type
     */
    void setObjectType(QObject* objectType);

    /**
     * Update the scene-graph.
     *
     * \see QQuickItem::updatePaintNode()
     */
    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* oldNodeData) override;

    /**
     * Draw the content.
     *
     * \param content - the content to be drawn
     * \param oldNode - the node drawn by the previous call if any
     *
     * \return the drawn node
     */
    QSGNode* drawContent(const core::CampaignMap::Content& content, QSGNode* oldNode) override;

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
     * Will emit the signal CampaignMapEditor::currentFactionChanged() if the
     * newly set value is different than the current one.
     *
     * \param currentFaction the current faction
     */
    void setCurrentFaction(core::Faction* currentFaction);

signals:
    /**
     * Emitted when the map-node changes.
     */
    void campaignMapChanged();

    /**
     * Emitted when the world-surface changes.
     */
    void worldSurfaceChanged();

    /**
     * Emitted when the editing-mode changes.
     */
    void editingModeChanged();

    /**
     * Emitted when the object-type changes.
     */
    void objectTypeChanged();

    /**
     * Emitted when the available object-types change.
     */
    void availableObjectTypesChanged();

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
    void doTerrainTypeEditingAction(const QPoint& pos);
    void doSettlementTypeEditingAction();
    void doArmyTypeEditingAction();
    void doGrantToCurrentFactionEditingAction();
    bool isCurrentEditingActionPossible() const;

    core::CampaignMap* campaignMap;
    WorldSurface* worldSurface;
    std::map<core::MapNode*, QPoint> mapNodesPos;

    core::MapNode* hoverMapNode;
    boost::optional<QPoint> hoverPos;

    EditingMode editingMode;
    QObject* objectType;
    core::Faction* currentFaction;

    CampaignMapWatcher* watcher;
};

} // namespace ui
} // namespace warmonger

#endif // W_UI_CAMPAIGN_MAP_EDITOR_H
