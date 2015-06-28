#ifndef CORE_ARMOR_H
#define CORE_ARMOR_H

#include <QMap>

#include "core/DamageType.h"

namespace core {

class Armor :
	public WorldItem
{
	Q_OBJECT

public:
	Armor(QObject *parent = nullptr);
	~Armor();

	int getRange() const;
	void setRange(int range);

	QMap<DamageType *, int> getDefenses() const;
	void setDefenses(const QMap<DamageType *, int> &damages);

	int getDefense(DamageType * const damageType) const;
	void setDefense(DamageType * const damageType, int defense);

	void fromJson(const QJsonObject &obj);
	QJsonObject toJson() const;

private:
	int range;
	QMap<DamageType *, int> defenses;
};

}; // namespace core

#endif // CORE_ARMOR_H
