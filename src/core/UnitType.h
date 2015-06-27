#ifndef CORE_UNIT_TYPE_H
#define CORE_UNIT_TYPE_H

#include "core/WorldElement.h"

namespace core {

class UnitClass :
	public WorldElement
{
	UnitClass(
		const QString &name,
		const QString &displayName,
		QObject *parent = nullptr
	);
	UnitClass(const QJonsObject &json, QObject *parent = nullptr);
	~UnitClass();
};

}; // namespace core

#endif // CORE_UNIT_TYPE_H
