#ifndef CORE_DAMAGE_TYPE_H
#define CORE_DAMAGE_TYPE_H

#include "core/WorldItem.h"

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

#endif // CORE_DAMAGE_TYPE_H
