#ifndef CORE_FACTION_H
#define CORE_FACTION_H

#include "core/GameObject.h"

namespace warmonger {
namespace core {

class UnitType;

class Faction :
    public GameObject
{
    Q_OBJECT

public:
    Faction(QObject *parent);
    ~Faction();

    QList<UnitType *> getUnitTypes() const;
    void setUnitTypes(const QList<UnitType *> &unitTypes);

private:
    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;

    QList<UnitType *> unitTypes;
};

} // namespace core
} // namespace warmonger

#endif // CORE_FACTION_H
