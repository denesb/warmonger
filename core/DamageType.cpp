#include "core/DamageType.h"

using namespace warmonger::core;

DamageType::DamageType(QObject *parent) :
    QObject(parent)
{
}

DamageType::~DamageType()
{
}

QString DamageType::getDisplayName() const
{
    return this->displayName;
}

void DamageType::setDisplayName(const QString &displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
}
