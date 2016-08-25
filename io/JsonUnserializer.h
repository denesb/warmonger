#ifndef IO_JSON_UNSERIALIZER_H
#define IO_JSON_UNSERIALIZER_H

#include "io/Unserializer.h"

namespace warmonger {
namespace io {

class JsonUnserializer :
    public Unserializer
{
public:
    using Unserializer::Unserializer;

    core::Army* unserializeArmy(const QByteArray& data) override;
    core::ArmyType* unserializeArmyType(const QByteArray& data) override;
    core::CampaignMap* unserializeCampaignMap(const QByteArray& data) override;
    core::Civilization* unserializeCivilization(const QByteArray& data) override;
    core::Faction* unserializeFaction(const QByteArray& data) override;
    core::MapNode* unserializeMapNode(const QByteArray& data) override;
    core::Settlement* unserializeSettlement(const QByteArray& data) override;
    core::SettlementType* unserializeSettlementType(const QByteArray& data) override;
    core::TerrainType* unserializeTerrainType(const QByteArray& data) override;
    core::Unit* unserializeUnit(const QByteArray& data) override;
    core::UnitType* unserializeUnitType(const QByteArray& data) override;
    core::World* unserializeWorld(const QByteArray& data) override;
};

} // namespace warmonger
} // namespace io

#endif // IO_JSON_UNSERIALIZER_H
