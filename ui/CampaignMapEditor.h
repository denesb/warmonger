#ifndef UI_CAMPAIGN_MAP_EDITOR_H
#define UI_CAMPAIGN_MAP_EDITOR_H

#include <map>

#include "core/CampaignMap.h"
#include "ui/BasicMap.h"
#include "ui/MapDrawer.h"
#include "ui/WorldSurface.h"

namespace warmonger {
namespace ui {

class CampaignMapWatcher;

class CampaignMapEditor :
    public BasicMap,
    public MapDrawer
{
    Q_OBJECT
    Q_PROPERTY(warmonger::core::CampaignMap* campaignMap READ getCampaignMap WRITE setCampaignMap NOTIFY campaignMapChanged)
    Q_PROPERTY(warmonger::ui::WorldSurface* worldSurface READ getWorldSurface WRITE setWorldSurface NOTIFY worldSurfaceChanged)
    Q_PROPERTY(warmonger::ui::CampaignMapEditor::EditingMode editingMode READ getEditingMode WRITE setEditingMode NOTIFY editingModeChanged)
    Q_PROPERTY(QObject* objectType READ getObjectType WRITE setObjectType NOTIFY objectTypeChanged)

public:
    enum class EditingMode
    {
        TerrainType,
        SettlementType
    };
    Q_ENUM(EditingMode)

    CampaignMapEditor(QQuickItem *parent = nullptr);

    core::CampaignMap* getCampaignMap() const;
    void setCampaignMap(core::CampaignMap* campaignMap);

    WorldSurface* getWorldSurface() const;
    void setWorldSurface(WorldSurface* worldSurface);

    EditingMode getEditingMode() const;
    void setEditingMode(EditingMode editingMode);

    QObject* getObjectType() const;
    void setObjectType(QObject* objectType);

    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* oldNodeData) override;

    QSGNode* drawMapNodeAndContents(const core::MapNode* mapNode, QSGNode* oldNode) override;

signals:
    void campaignMapChanged();
    void worldSurfaceChanged();
    void editingModeChanged();
    void objectTypeChanged();

protected:
    void hoverMoveEvent(QHoverEvent* event) override;
    void hoverLeaveEvent(QHoverEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    void updateContent();
    void updateMapRect();
    void doEditingAction(const QPoint& pos);
    void doTerrainTypeEditingAction(const QPoint& pos);
    void doSettlementTypeEditingAction();

    core::CampaignMap *campaignMap;
    WorldSurface *worldSurface;
    std::map<const core::MapNode*, QPoint> mapNodesPos;
    core::MapNode* currentMapNode;

    EditingMode editingMode;
    QObject* objectType;

    CampaignMapWatcher* watcher;
};

} // namespace ui
} // namespace warmonger

#endif // UI_CAMPAIGN_MAP_EDITOR_H
