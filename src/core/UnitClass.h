#ifndef CORE_UNIT_CLASS_H
#define CORE_UNIT_CLASS_H

#include <QMap>

#include "core/TerrainType.h"

namespace core {

class UnitClass :
	public WorldItem
{
public:
	typedef QMap<TerrainType *, int> TerrainInfluenceMap;

	UnitClass(QObject *parent = nullptr);
	~UnitClass();

	int getMovementPoints() const;
	void setMovementPoints(int movementPoints);

	TerrainInfluenceMap getMovements() const;
	void setMovements(const TerrainInfluenceMap &movements);

	int getMovement(TerrainType *terrainType) const;
	void setMovement(TerrainType *terrainType, int movement);

	TerrainInfluenceMap getAttacks() const;
	void setAttacks(const TerrainInfluenceMap &attacks);

	int getAttack(TerrainType *terrainType) const;
	void setAttack(TerrainType *terrainType, int attack);

	TerrainInfluenceMap getDefenses() const;
	void setDefenses(const TerrainInfluenceMap &defenses);

	int getDefense(TerrainType *terrainType) const;
	void setDefense(TerrainType *terrainType, int defense);

	WorldItem * getWorldItem(const QString &className, const QString &objectName) const;

	void fromJson(const QJsonObject &json);
	QJsonObject toJson() const;

private:
	TerrainInfluenceMap terrainInfluenceMapFromJson(const QJsonObject &obj);
	QJsonObject terrainInfluenceMapToJson(const TerrainInfluenceMap &influenceMap) const;

	int movementPoints;
	TerrainInfluenceMap movements;
	TerrainInfluenceMap attacks;
	TerrainInfluenceMap defenses;
};

}; // namespace core

#endif // CORE_UNIT_CLASS_H
