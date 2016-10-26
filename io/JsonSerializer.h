#ifndef IO_JSON_SERIALIZER_H
#define IO_JSON_SERIALIZER_H

#include <QJsonDocument>

#include "io/Serializer.h"

namespace warmonger {
namespace io {

class JsonSerializer : public Serializer
{
public:
    JsonSerializer(QJsonDocument::JsonFormat format = QJsonDocument::Indented);

    QByteArray serializeArmy(const core::Army* obj) override;
    QByteArray serializeArmyType(const core::ArmyType* obj) override;
    QByteArray serializeBanner(const core::Banner* obj) override;
    QByteArray serializeCampaignMap(const core::CampaignMap* obj) override;
    QByteArray serializeCivilization(const core::Civilization* obj) override;
    QByteArray serializeFaction(const core::Faction* obj) override;
    QByteArray serializeMapNode(const core::MapNode* obj) override;
    QByteArray serializeSettlement(const core::Settlement* obj) override;
    QByteArray serializeSettlementType(const core::SettlementType* obj) override;
    QByteArray serializeTerrainType(const core::TerrainType* obj) override;
    QByteArray serializeUnit(const core::Unit* obj) override;
    QByteArray serializeUnitType(const core::UnitType* obj) override;
    QByteArray serializeWorld(const core::World* obj) override;

private:
    QJsonDocument::JsonFormat format;
};

} // namespace warmonger
} // namespace io

#endif // IO_JSON_SERIALIZER_H
