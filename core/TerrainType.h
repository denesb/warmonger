#ifndef TERRAIN_TYPE_WORLD_H
#define TERRAIN_TYPE_WORLD_H

#include "core/GameObject.h"

namespace warmonger {
namespace core {

class TerrainType :
    public GameObject
{
    Q_OBJECT

public:
    TerrainType(QObject *parent);
    ~TerrainType();

private:
    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;
};

} // namespace core
} // namespace warmonger

#endif // TERRAIN_TYPE_WORLD_H
