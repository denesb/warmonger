#ifndef CORE_WORLD_H
#define CORE_WORLD_H

#include "core/TerrainType.h"
#include "core/UnitClass.h"

namespace core {

class World :
    public WorldItem
{
    Q_OBJECT

public:
	World(QObject *parent = nullptr);

	int getVillageGoldPt() const;
	void setVillageGoldPt(int villageGoldPt);

	int getCityGoldPt() const;
	void setCityGoldPt(int cityGoldPt);

	WorldItem * getWorldItem(const QString &className, const QString &objectName) const;

	void fromJson(const QJsonObject &obj);
	QJsonObject toJson() const;

private:
	int villageGoldPt;
	int cityGoldPt;
	QList<TerrainType *> terrainTypes;
	QList<UnitClass *> unitClasses;
};

}; // namespace core

#endif // CORE_WORLD_H
