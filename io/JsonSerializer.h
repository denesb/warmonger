#ifndef IO_JSON_SERIALIZER_H
#define IO_JSON_SERIALIZER_H

#include <QJsonDocument>
#include <QJsonObject>

#include "io/Serializer.h"

namespace warmonger {
namespace core {
    class GameObject;
}

namespace io {

class JsonSerializer :
    public Serializer
{
public:
    JsonSerializer(QJsonDocument::JsonFormat format = QJsonDocument::Indented);

    virtual QByteArray serialize(const core::Armor *obj);
    virtual QByteArray serialize(const core::DamageType *obj);
    virtual QByteArray serialize(const core::Faction *obj);
    virtual QByteArray serialize(const core::Game *obj);
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
    QJsonObject toJson(const core::Armor *obj);
    QJsonObject toJson(const core::DamageType *obj);
    QJsonObject toJson(const core::Faction *obj);
    QJsonObject toJson(const core::MapNode *obj);
    QJsonObject toJson(const core::Player *obj);
    QJsonObject toJson(const core::SettlementType *obj);
    QJsonObject toJson(const core::TerrainType *obj);
    QJsonObject toJson(const core::Unit *obj);
    QJsonObject toJson(const core::UnitClass *obj);
    QJsonObject toJson(const core::UnitLevel *obj);
    QJsonObject toJson(const core::UnitType *obj);
    QJsonObject toJson(const core::Weapon *obj);

    QJsonObject gameObjectToJson(const core::GameObject *obj);

    QJsonDocument::JsonFormat format;
};

} // namespace warmonger
} // namespace io

#endif // IO_JSON_SERIALIZER_H
