#include "core/WeaponClass.h"

namespace warmonger {
namespace core {

WeaponClass::WeaponClass(QObject *parent) :
    QObject(parent),
    displayName("")
{
}

QString WeaponClass::getDisplayName() const
{
    return this->displayName;
}

void WeaponClass::setDisplayName(const QString &displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
}

} // namespace core
} // namespace warmonger
