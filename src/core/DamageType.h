#ifndef CORE_DAMAGE_TYPE_H
#define CORE_DAMAGE_TYPE_H

#include "core/WorldItem.h"

namespace warmonger {
namespace core {

class DamageType :
    public WorldItem
{
    Q_OBJECT

public:
    DamageType(QObject *parent = nullptr);
    ~DamageType();
};

}; // namespace core
}; // namespace warmonger

#endif // CORE_DAMAGE_TYPE_H
