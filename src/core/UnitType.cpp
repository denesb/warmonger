#include "core/UnitType.h"

using namespace core;

UnitType::UnitType(const QJsonObject &obj, QObject *parent) :
	WorldElement(name, displayName, parent)
{
}

UnitType::UnitType(const QJsonObject &obj, QObject *parent) :
	WorldElement(obj, parent)
{
}

UnitType::~UnitType()
{
}
