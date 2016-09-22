#include "core/CampaignMap.h"
#include "ui/CampaignMapWatcher.h"

namespace warmonger {
namespace ui {

CampaignMapWatcher::CampaignMapWatcher(const core::CampaignMap* const campaignMap, QObject* parent)
    : QObject(parent)
    , campaignMap(campaignMap)
    , mapNodeWatcher(new Watcher(this))
    , settlementWatcher(new Watcher(this))
{
    QObject::connect(
        this->campaignMap, &core::CampaignMap::mapNodesChanged, this, &CampaignMapWatcher::onMapNodesChanged);
    QObject::connect(
        this->campaignMap, &core::CampaignMap::settlementsChanged, this, &CampaignMapWatcher::onSettlementsChanged);

    QObject::connect(this->mapNodeWatcher, &Watcher::changed, this, &CampaignMapWatcher::changed);
    QObject::connect(this->settlementWatcher, &Watcher::changed, this, &CampaignMapWatcher::changed);

    this->connectMapNodeSignals();
    this->connectSettlementSignals();
}

void CampaignMapWatcher::connectMapNodeSignals()
{
    const std::vector<core::MapNode*> mapNodes = this->campaignMap->getMapNodes();
    for (core::MapNode* mapNode : mapNodes)
    {
        QObject::connect(mapNode, &core::MapNode::terrainTypeChanged, this->mapNodeWatcher, &Watcher::changed);
    }
}

void CampaignMapWatcher::connectSettlementSignals()
{
    const std::vector<core::Settlement*> settlements = this->campaignMap->getSettlements();
    for (core::Settlement* settlement : settlements)
    {
        QObject::connect(settlement, &core::Settlement::typeChanged, this->settlementWatcher, &Watcher::changed);
    }
}

void CampaignMapWatcher::onMapNodesChanged()
{
    QObject::disconnect(this->campaignMap, nullptr, this->mapNodeWatcher, nullptr);
    this->connectMapNodeSignals();

    emit changed();
}

void CampaignMapWatcher::onSettlementsChanged()
{
    QObject::disconnect(this->campaignMap, nullptr, this->settlementWatcher, nullptr);
    this->connectSettlementSignals();

    emit changed();
}

} // namespace ui
} // namespace warmonger
