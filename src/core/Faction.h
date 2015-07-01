#ifndef CORE_FACTION_H
#define CORE_FACTION_H

#include "core/UnitType.h"

namespace core {

class Faction :
    public WorldItem
{
    Q_OBJECT

public:
    Faction(QObject *parent = nullptr);
    ~Faction();

    QList<UnitType *> getUnitTypes() const;
    void setUnitTypes(const QList<UnitType *> &unitTypes);

    void fromJson(const QJsonObject &obj);
    QJsonObject toJson() const;

private:
    QList<UnitType *> unitTypes;
};

}; // namespace core

#endif // CORE_FACTION_H
