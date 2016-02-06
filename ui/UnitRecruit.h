#ifndef UI_UNIT_RECRUIT_H
#define UI_UNIT_RECRUIT_H

#include <QHash>
#include <QLineF>
#include <QRect>
#include <QtQuick/QQuickPaintedItem>

#include "core/Game.h"
#include "core/MapNode.h"
#include "core/Settlement.h"
#include "core/UnitType.h"
#include "ui/MapDrawer.h"

namespace warmonger {

namespace core {
    class World;
    class WorldSurface;
}

namespace ui {

class MapDrawer;

class UnitRecruit :
    public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(warmonger::core::Game *game READ getGame WRITE setGame NOTIFY gameChanged)
    Q_PROPERTY(warmonger::core::Settlement *settlement READ getSettlement WRITE setSettlement NOTIFY settlementChanged)
    Q_PROPERTY(warmonger::core::UnitType *unitType READ getUnitType WRITE setUnitType NOTIFY unitTypeChanged)
    Q_PROPERTY(QString error READ getError NOTIFY errorChanged)
    Q_PROPERTY(bool canRecruit READ getCanRecruit NOTIFY canRecruitChanged)

public:
    UnitRecruit(QQuickItem *parent = nullptr);
    ~UnitRecruit();

    core::Game * getGame() const;
    void setGame(core::Game *game);

    core::Settlement * getSettlement() const;
    void setSettlement(core::Settlement *settlement);

    core::UnitType * getUnitType() const;
    void setUnitType(core::UnitType *unitType);

    QString getError() const;

    bool getCanRecruit() const;

    void paint(QPainter *painter);

signals:
    void gameChanged();
    void settlementChanged();
    void unitTypeChanged();
    void errorChanged();
    void canRecruitChanged();

public slots:
    void recruitUnit();

private slots:
    void onUnitAdded(const core::Unit *unit);

private:
    void mousePressEvent(QMouseEvent *event);

    void setError(const QString &error);

    void updateCanRecruit();

    void cleanup();
    void setupMap();
    void updateGeometry();
    void updateTransform();
    bool rectContainsNode(const QRect &rect, const core::MapNode *node);
    QPointF mapToMap(const QPointF &p) const;

    void setFocusedNode(core::MapNode *node);
    void updateFocus(const QPoint &p);

    bool canRecruitOnNode(const core::MapNode *node) const;

    void selectFocusNode();

    core::Game *game;
    core::World *world;
    core::WorldSurface *surface;
    QSize tileSize;

    core::Settlement *settlement;
    core::UnitType *unitType;

    MapDrawer *mapDrawer;

    QList<core::MapNode *> nodes;
    QHash<const core::MapNode *, QPoint> nodesPos;

    core::MapNode *focusedNode;

    QRect boundingRect;

    qreal scale;
    QPointF translate;

    QString error;
    bool canRecruit;
};

} // namespace ui
} // namespace warmonger

#endif // UI_UNIT_RECRUIT_H
