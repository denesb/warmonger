#include "core/TerrainType.h"

using namespace warmonger::core;

TerrainType::TerrainType(QObject *parent) :
    GameObject(parent)
{
}

TerrainType::~TerrainType()
{
}

void TerrainType::dataFromJson(const QJsonObject &obj)
{
    Q_UNUSED(obj);
}

void TerrainType::dataToJson(QJsonObject &obj) const
{
    Q_UNUSED(obj);
}
