#ifndef IO_JSON_SERIALIZER_H
#define IO_JSON_SERIALIZER_H

#include <QJsonDocument>

#include "io/Serializer.h"

namespace warmonger {
namespace io {

class JsonSerializer :
    public Serializer
{
public:
    JsonSerializer(QJsonDocument::JsonFormat format = QJsonDocument::Indented);

    QByteArray serializeArmor(const core::Armor *obj) override;
    QByteArray serializeArmy(const core::Army *obj) override;
    QByteArray serializeCampaignMap(const core::CampaignMap *obj) override;
    QByteArray serializeCivilization(const core::Civilization *obj) override;
    QByteArray serializeDamageType(const core::DamageType *obj) override;
    QByteArray serializeFaction(const core::Faction *obj) override;
    QByteArray serializeMapNode(const core::MapNode *obj) override;
    QByteArray serializeSettlement(const core::Settlement *obj) override;
    QByteArray serializeSettlementType(const core::SettlementType *obj) override;
    QByteArray serializeTerrainType(const core::TerrainType *obj) override;
    QByteArray serializeUnit(const core::Unit *obj) override;
    QByteArray serializeUnitClass(const core::UnitClass *obj) override;
    QByteArray serializeUnitLevel(const core::UnitLevel *obj) override;
    QByteArray serializeUnitType(const core::UnitType *obj) override;
    QByteArray serializeWeapon(const core::Weapon *obj) override;
    QByteArray serializeWorld(const core::World *obj) override;
    QByteArray serializeWorldSurface(const core::WorldSurface *obj) override;

private:
    QJsonDocument::JsonFormat format;
};

} // namespace warmonger
} // namespace io

#endif // IO_JSON_SERIALIZER_H
