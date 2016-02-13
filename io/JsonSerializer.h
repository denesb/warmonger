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

    virtual QByteArray serialize(const core::Armor *obj);
    virtual QByteArray serialize(const core::DamageType *obj);
    virtual QByteArray serialize(const core::Faction *obj);
    virtual QByteArray serialize(const core::Map *obj);
    virtual QByteArray serialize(const core::MapNode *obj);
    virtual QByteArray serialize(const core::Player *obj);
    virtual QByteArray serialize(const core::Settlement *obj);
    virtual QByteArray serialize(const core::SettlementType *obj);
    virtual QByteArray serialize(const core::TerrainType *obj);
    virtual QByteArray serialize(const core::Unit *obj);
    virtual QByteArray serialize(const core::UnitClass *obj);
    virtual QByteArray serialize(const core::UnitLevel *obj);
    virtual QByteArray serialize(const core::UnitType *obj);
    virtual QByteArray serialize(const core::Weapon *obj);
    virtual QByteArray serialize(const core::World *obj);
    virtual QByteArray serialize(const core::WorldSurface *obj);

private:
    QJsonDocument::JsonFormat format;
};

} // namespace warmonger
} // namespace io

#endif // IO_JSON_SERIALIZER_H
