#include "core/CampaignMap.h"
#include "ui/CampaignMapWatcher.h"

namespace warmonger {
namespace ui {

CampaignMapWatcher::CampaignMapWatcher(const core::CampaignMap* const campaignMap, QObject* parent) :
    QObject(parent),
    campaignMap(campaignMap)
{
    this->connectSignals();
    QObject::connect(
            this->campaignMap,
            &core::CampaignMap::mapNodesChanged,
            this,
            &CampaignMapWatcher::onMapNodesChanged);
}

void CampaignMapWatcher::connectSignals()
{
    const std::vector<core::MapNode*> mapNodes = this->campaignMap->getMapNodes();
    for (core::MapNode* mapNode : mapNodes)
    {
        QObject::connect(mapNode, &core::MapNode::terrainTypeChanged, this, &CampaignMapWatcher::changed);
    }
}

void CampaignMapWatcher::onMapNodesChanged()
{
    QObject::disconnect(this->campaignMap, nullptr, this, nullptr);
    this->connectSignals();
}

} // namespace ui
} // namespace warmonger
