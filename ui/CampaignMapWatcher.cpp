#include "core/CampaignMap.h"
#include "ui/CampaignMapWatcher.h"

namespace warmonger {
namespace ui {

CampaignMapWatcher::CampaignMapWatcher(const core::CampaignMap* const campaignMap, QObject* parent) :
    QObject(parent),
    campaignMap(campaignMap)
{
    const std::vector<core::MapNode*> mapNodes = this->campaignMap->getMapNodes();
    for (core::MapNode* mapNode : mapNodes)
    {
        QObject::connect(mapNode, &core::MapNode::terrainTypeChanged, this, &CampaignMapWatcher::changed);
        QObject::connect(mapNode, &core::MapNode::neighboursChanged, this, &CampaignMapWatcher::changed);
    }
}

} // namespace ui
} // namespace warmonger
