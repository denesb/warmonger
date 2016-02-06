#include "core/DamageType.h"

using namespace warmonger::core;

DamageType::DamageType(QObject *parent) :
    GameObject(parent)
{
}

DamageType::~DamageType()
{
}

void DamageType::dataFromJson(const QJsonObject &obj)
{
    Q_UNUSED(obj);
}

void DamageType::dataToJson(QJsonObject &obj) const
{
    Q_UNUSED(obj);
}
