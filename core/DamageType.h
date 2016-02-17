#ifndef CORE_DAMAGE_TYPE_H
#define CORE_DAMAGE_TYPE_H

#include "core/GameObject.h"

namespace warmonger {
namespace core {

class DamageType :
    public GameObject
{
    Q_OBJECT

public:
    DamageType(QObject *parent=nullptr);
    ~DamageType();

private:
    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;
};

} // namespace core
} // namespace warmonger

#endif // CORE_DAMAGE_TYPE_H
