#ifndef CORE_FACTION_H
#define CORE_FACTION_H

#include "core/WorldItem.h"

namespace warmonger {
namespace core {

class UnitType;

class Faction :
    public WorldItem
{
    Q_OBJECT

public:
    Faction(QObject *parent);
    ~Faction();

    QList<const UnitType *> getUnitTypes() const;
    void setUnitTypes(const QList<const UnitType *> &unitTypes);

    void fromJson(const QJsonObject &obj);
    QJsonObject toJson() const;

private:
    QList<const UnitType *> unitTypes;
};

}; // namespace core
}; // namespace warmonger

#endif // CORE_FACTION_H
