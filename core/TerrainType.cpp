#include "core/TerrainType.h"

using namespace warmonger::core;

TerrainType::TerrainType(QObject* parent)
    : QObject(parent)
{
}

TerrainType::~TerrainType()
{
}

QString TerrainType::getDisplayName() const
{
    return this->displayName;
}

void TerrainType::setDisplayName(const QString& displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
}
