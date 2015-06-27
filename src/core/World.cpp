#include "core/World.h"

using namespace core;

World::World(QObject *parent) :
	WorldItem(parent)
{
}

int World::getVillageGoldPt() const
{
	return this->villageGoldPt;
}

void World::setVillageGoldPt(int villageGoldPt)
{
	this->villageGoldPt = villageGoldPt;
}

int World::getCityGoldPt() const
{
	return this->cityGoldPt;
}

void World::setCityGoldPt(int cityGoldPt)
{
	this->cityGoldPt = cityGoldPt;
}

WorldItem * World::getWorldItem(const QString &className, const QString &objectName) const
{
	//TODO implement
	return nullptr;
}

void World::fromJson(const QJsonObject &obj)
{
	this->villageGoldPt = obj["villageGoldPt"].toInt();
	this->cityGoldPt = obj["villageGoldPt"].toInt();
	this->terrainTypes = this->propertyListFromJson<TerrainType>(obj["terrainTypes"].toArray());
	this->unitClasses = this->propertyListFromJson<UnitClass>(obj["unitClasses"].toArray());
}

QJsonObject World::toJson() const
{
	QJsonObject &&obj = WorldItem::toJson();

	obj["villageGoldPt"] = this->villageGoldPt;
	obj["cityGoldPt"] = this->cityGoldPt;

	return std::move(obj);
}
