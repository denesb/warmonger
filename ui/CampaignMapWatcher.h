#ifndef W_UI_CAMPAIGN_MAP_WATCHER_H
#define W_UI_CAMPAIGN_MAP_WATCHER_H

#include <QObject>

namespace warmonger {

namespace core {

class CampaignMap;
}

namespace ui {

struct Watcher : public QObject
{
    Q_OBJECT

    using QObject::QObject;

signals:
    void changed();
};

class CampaignMapWatcher : public QObject
{
    Q_OBJECT

public:
    CampaignMapWatcher(const core::CampaignMap* const campaignMap, QObject* parent = nullptr);

signals:
    void changed();

private:
    void connectMapNodeSignals();
    void connectSettlementSignals();
    void onMapNodesChanged();
    void onSettlementsChanged();

private:
    const core::CampaignMap* const campaignMap;
    Watcher* mapNodeWatcher;
    Watcher* settlementWatcher;
};

} // namespace ui
} // namespace warmonger

#endif // W_UI_CAMPAIGN_MAP_WATCHER_H
