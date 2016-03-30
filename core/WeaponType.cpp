#include "core/WeaponType.h"

const QString loggerName{"core.WeaponType"};

namespace warmonger {
namespace core {

WeaponType::WeaponType(QObject *parent) :
    QObject(parent),
    displayName(""),
    klass(nullptr)
{
}

QString WeaponType::getDisplayName() const
{
    return this->displayName;
}

void WeaponType::setDisplayName(const QString &displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
}

WeaponClass * WeaponType::getClass() const
{
    return this->klass;
}

void WeaponType::setClass(WeaponClass *klass)
{
    if (this->klass != klass)
    {
        this->klass = klass;
        emit classChanged();
    }
}

} // namespace core
} // namespace warmonger
