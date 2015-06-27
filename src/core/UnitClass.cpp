#include "core/UnitClass.h"

using namespace core;

UnitClass::UnitClass(QObject *parent) :
	WorldItem(parent),
	movementPoints(0),
	movements(),
	attacks(),
	defenses()
{
}

UnitClass::~UnitClass()
{
}

int UnitClass::getMovementPoints() const
{
	return this->movementPoints;
}

void UnitClass::setMovementPoints(int movementPoints)
{
	this->movementPoints = movementPoints;
}

UnitClass::TerrainInfluenceMap UnitClass::getMovements() const
{
	return this->movements;
}

void UnitClass::setMovements(const UnitClass::TerrainInfluenceMap &movements)
{
	this->movements = movements;
}

int UnitClass::getMovement(TerrainType *terrainType) const
{
	return this->movements[terrainType];
}

void UnitClass::setMovement(TerrainType *terrainType, int movement)
{
	this->movements[terrainType] = movement;
}

UnitClass::TerrainInfluenceMap UnitClass::getAttacks() const
{
	return this->attacks;
}

void UnitClass::setAttacks(const UnitClass::TerrainInfluenceMap &attacks)
{
	this->attacks = attacks;
}

int UnitClass::getAttack(TerrainType *terrainType) const
{
	return this->attacks[terrainType];
}

void UnitClass::setAttack(TerrainType *terrainType, int attack)
{
	this->attacks[terrainType] = attack;
}

UnitClass::TerrainInfluenceMap UnitClass::getDefenses() const
{
	return this->defenses;
}

void UnitClass::setDefenses(const UnitClass::TerrainInfluenceMap &defenses)
{
	this->defenses = defenses;
}

int UnitClass::getDefense(TerrainType *terrainType) const
{
	return this->defenses[terrainType];
}

void UnitClass::setDefense(TerrainType *terrainType, int defense)
{
	this->defenses[terrainType] = defense;
}

WorldItem * UnitClass::getWorldItem(const QString &className, const QString &objectName) const
{
	Q_UNUSED(className);
	Q_UNUSED(objectName);
	return nullptr;
}

void UnitClass::fromJson(const QJsonObject &obj)
{
	WorldItem::fromJson(obj);
	this->movementPoints = obj["movementPoints"].toInt();
	this->movements = this->terrainInfluenceMapFromJson(obj["terrainMovements"].toObject());
	this->attacks = this->terrainInfluenceMapFromJson(obj["terrainAttacks"].toObject());
	this->defenses = this->terrainInfluenceMapFromJson(obj["terrainDefenses"].toObject());
}

QJsonObject UnitClass::toJson() const
{
	QJsonObject &&obj = WorldItem::toJson();

	obj["movementPoints"] = this->movementPoints;
	obj["terrainMovements"] = this->terrainInfluenceMapToJson(this->movements);
	obj["terrainAttacks"] = this->terrainInfluenceMapToJson(this->attacks);
	obj["terrainDefenses"] = this->terrainInfluenceMapToJson(this->defenses);

	return std::move(obj);
}

UnitClass::TerrainInfluenceMap UnitClass::terrainInfluenceMapFromJson(const QJsonObject &obj)
{
	TerrainInfluenceMap influenceMap;
	WorldItem *parent = qobject_cast<WorldItem *>(this->parent());

	QJsonObject::const_iterator it;
    for(it = obj.constBegin(); it != obj.constEnd(); it++)
	{
		const QString terrainTypeName = it.key();
		WorldItem *worldItem = parent->getWorldItem("TerrainType", terrainTypeName);
		TerrainType *terrainType = qobject_cast<TerrainType *>(worldItem);

		influenceMap[terrainType] = it.value().toInt();
	}

	return std::move(influenceMap);
}

QJsonObject UnitClass::terrainInfluenceMapToJson(const UnitClass::TerrainInfluenceMap &influenceMap) const
{
	QJsonObject obj;

	TerrainInfluenceMap::const_iterator it;
    for(it = influenceMap.constBegin(); it != influenceMap.constEnd(); it++)
	{
		TerrainType *terrainType = it.key();
		obj[terrainType->objectName()] = it.value();
	}
	
	return std::move(obj);
}
