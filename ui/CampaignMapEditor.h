#ifndef UI_CAMPAIGN_MAP_EDITOR_H
#define UI_CAMPAIGN_MAP_EDITOR_H

#include <QHash>
#include <QLineF>
#include <QRect>
#include <QtQuick/QQuickPaintedItem>

#include "core/CampaignMap.h"
#include "ui/MapDrawer.h"

namespace warmonger {

namespace core {
    class World;
    class WorldSurface;
}

namespace ui {

class MapDrawer;
struct MovingUnit;

class CampaignMapEditor :
    public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(warmonger::core::CampaignMap *campaignMap READ getMap WRITE setMap NOTIFY mapChanged)
    Q_PROPERTY(QPoint windowPos READ getWindowPos WRITE setWindowPos NOTIFY windowPosChanged)
    Q_PROPERTY(QSize windowSize READ getWindowSize NOTIFY windowSizeChanged)
    Q_PROPERTY(Mode mode READ getMode WRITE setMode NOTIFY modeChanged)

public:
    CampaignMapEditor(QQuickItem *parent = nullptr);
    ~CampaignMapEditor();

    core::CampaignMap *getMap() const;
    void setMap(core::CampaignMap *map);

    QPoint getWindowPos() const;
    void setWindowPos(const QPoint &windowPos);

    void centerWindow(const QPoint &pos);
    void moveWindowBy(const QPoint &diff);

    QSize getWindowSize() const;

    Mode getMode() const;
    void setMode(Mode mode);

    void paint(QPainter *painter);

signals:
    void currentMapNodeChanged();
    void currentSettlementChanged();
    void currentUnitChanged();
    void focusedMapNodeChanged();
    void focusedSettlementChanged();
    void focusedUnitChanged();
    void gameChanged();
    void modeChanged();
    void windowPosChanged();
    void windowSizeChanged();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void hoverMoveEvent(QHoverEvent *event);

private slots:
    void onUnitAdded(const core::Unit *unit);

private:
    void setupMap();
    void updateGeometry();
    void updateWindowPosRect();
    bool rectContainsNode(const QRect &rect, const core::MapNode *node);
    QPoint mapToMap(const QPoint &p);

    void onWidthChanged();
    void onHeightChanged();
    void onFocusedNodeChanged();
    void onCurrentNodeChanged();

    void updateFocus(const QPoint &p);
    void moveUnit(const QPoint &p);
    void advanceUnits();

    QList<DrawingInfo::Overlay> getMapOverlays() const;
    QList<DrawingInfo::Overlay> getMovementMapOverlays() const;
    QList<DrawingInfo::Overlay> getRecruitmentMapOverlays() const;
    QList<DrawingInfo::Overlay> getBattleMapOverlays() const;

    core::Game *game;
    core::World *world;
    core::WorldSurface *surface;
    QSize tileSize;

    MapDrawer *mapDrawer;

    QList<core::MapNode *> nodes;
    QHash<const core::MapNode *, QPoint> nodesPos;
    QSet<core::MapNode *> reachableNodes;
    QSet<core::MapNode *> pathNodes;

    core::MapNode *focusedNode;
    core::MapNode *currentNode;

    QTimer *unitMoveTimer;
    QList<MovingUnit *> movingUnits;

    QRect boundingRect;
    QRect windowPosRect;
    QPoint windowPos;
    QSize windowSize;
    QPoint lastPos;

    Mode mode;
};

} // namespace ui
} // namespace warmonger

#endif // UI_CAMPAIGN_MAP_EDITOR_H
