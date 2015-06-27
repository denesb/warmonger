#include "core/TerrainType.h"

using namespace core;

TerrainType::TerrainType(QObject *parent) :
	WorldItem(parent)
{
}

TerrainType::~TerrainType()
{
}

WorldItem * TerrainType::getWorldItem(const QString &className, const QString &objectName) const
{
	Q_UNUSED(className);
	Q_UNUSED(objectName);
	return nullptr;
}
