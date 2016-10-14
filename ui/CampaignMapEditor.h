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

class CampaignMapEditor : public BasicMap, public CampaignMapDrawer
{
    Q_OBJECT
    Q_PROPERTY(
        warmonger::core::CampaignMap* campaignMap READ getCampaignMap WRITE setCampaignMap NOTIFY campaignMapChanged)
    Q_PROPERTY(
        warmonger::ui::WorldSurface* worldSurface READ getWorldSurface WRITE setWorldSurface NOTIFY worldSurfaceChanged)
    Q_PROPERTY(warmonger::ui::CampaignMapEditor::EditingMode editingMode READ getEditingMode WRITE setEditingMode NOTIFY
            editingModeChanged)
    Q_PROPERTY(QVariantList availableObjectTypes READ readAvailableObjectTypes NOTIFY availableObjectTypesChanged)
    Q_PROPERTY(QObject* objectType READ getObjectType WRITE setObjectType NOTIFY objectTypeChanged)

public:
    enum class EditingMode
    {
        None,
        TerrainType,
        SettlementType,
        ArmyType,
        Edit,
        Remove
    };
    Q_ENUM(EditingMode)

    CampaignMapEditor(QQuickItem* parent = nullptr);

    core::CampaignMap* getCampaignMap() const;
    void setCampaignMap(core::CampaignMap* campaignMap);

    WorldSurface* getWorldSurface() const;
    void setWorldSurface(WorldSurface* worldSurface);

    EditingMode getEditingMode() const;
    void setEditingMode(EditingMode editingMode);

    QVariantList readAvailableObjectTypes() const;

    QObject* getObjectType() const;
    void setObjectType(QObject* objectType);

    QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* oldNodeData) override;

    QSGNode* drawContent(const core::CampaignMap::Content& content, QSGNode* oldNode) override;

signals:
    void campaignMapChanged();
    void worldSurfaceChanged();
    void editingModeChanged();
    void objectTypeChanged();
    void availableObjectTypesChanged();

protected:
    void hoverMoveEvent(QHoverEvent* event) override;
    void hoverLeaveEvent(QHoverEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    void updateContent();
    void updateMapRect();
    void onMapNodesChanged();
    void doEditingAction(const QPoint& pos);
    void doTerrainTypeEditingAction(const QPoint& pos);
    void doSettlementTypeEditingAction(const QPoint& pos);
    QSGNode* drawHoverNode(QSGNode* oldNode) const;

    core::CampaignMap* campaignMap;
    WorldSurface* worldSurface;
    std::map<core::MapNode*, QPoint> mapNodesPos;

    core::MapNode* hoverMapNode;
    boost::optional<QPoint> hoverPos;

    EditingMode editingMode;
    QObject* objectType;

    CampaignMapWatcher* watcher;
};

} // namespace ui
} // namespace warmonger

#endif // W_UI_CAMPAIGN_MAP_EDITOR_H
