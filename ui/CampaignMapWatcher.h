#ifndef W_UI_CAMPAIGN_MAP_WATCHER_H
#define W_UI_CAMPAIGN_MAP_WATCHER_H

#include <QObject>

namespace warmonger {

namespace core {

class CampaignMap;

}

namespace ui {

class CampaignMapWatcher : public QObject
{
    Q_OBJECT

public:
    CampaignMapWatcher(const core::CampaignMap* const campaignMap, QObject* parent = nullptr);

signals:
    void changed();

private:
    const core::CampaignMap* const campaignMap;
};

} // namespace ui
} // namespace warmonger

#endif // W_UI_CAMPAIGN_MAP_WATCHER_H
