#include "core/Armor.h"
#include "core/JsonUtil.hpp"

using namespace core;

Armor::Armor(QObject *parent) :
	WorldItem(parent)
{
}

Armor::~Armor()
{
}

int Armor::getRange() const
{
	return this->range;
}

void Armor::setRange(int range)
{
	this->range = range;
}

QMap<DamageType *, int> Armor::getDefenses() const
{
	return this->defenses;
}

void Armor::setDefenses(const QMap<DamageType *, int> &defenses)
{
	this->defenses = defenses;
}

int Armor::getDefense(DamageType * const damageType) const
{
	return this->defenses[damageType];
}

void Armor::setDefense(DamageType * const damageType, int defense)
{
	this->defenses[damageType] = defense;
}

void Armor::fromJson(const QJsonObject &obj)
{
	WorldItem::fromJson(obj);
	this->range = obj["range"].toInt();
	this->defenses = objectValueMapFromJson<DamageType>(obj["defenses"].toObject(), this);
}

QJsonObject Armor::toJson() const
{
	QJsonObject &&obj = WorldItem::toJson();

	obj["range"] = this->range;
	obj["defenses"] = objectValueMapToJson<DamageType>(this->defenses);

	return obj;
}
