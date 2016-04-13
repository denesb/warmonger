#ifndef UI_APPLICATION_CONTEXT_H
#define UI_APPLICATION_CONTEXT_H

#include <QVariant>
#include <QUrl>
#include <QRect>

#include "core/Game.h"
#include "core/CampaignMap.h"
#include "core/MapNode.h"
#include "core/Settlement.h"
#include "core/World.h"
#include "core/Unit.h"

namespace warmonger {
namespace ui {

class ApplicationContext :
    public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList maps READ readMaps NOTIFY mapsChanged)
    Q_PROPERTY(warmonger::core::Game *game READ getGame NOTIFY gameChanged)

public:
    ApplicationContext(QObject *parent=nullptr);
    ~ApplicationContext();

    QVariantList readMaps() const;
    core::CampaignMap * getMap() const;
    core::Game * getGame() const;
    core::World * getWorld() const;

public slots:
    void loadMaps();
    void closeMaps();

    void newGame(warmonger::core::CampaignMap *map);
    void loadGame(QString objectName);
    void closeGame();

signals:
    void mapsChanged();
    void gameChanged();

private:
    void setWorld(core::World *world);

private:
    QList<core::CampaignMap *> maps;
    core::Game *game;
};

} // namespace ui
} // namespace warmonger

#endif // UI_APPLICATION_CONTEXT_H
