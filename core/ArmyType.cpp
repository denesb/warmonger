#include "core/ArmyType.h"

namespace warmonger {
namespace core {

ArmyType::ArmyType(QObject* parent)
    : QObject(parent)
{
}

QString ArmyType::getDisplayName() const
{
    return this->displayName;
}

void ArmyType::setDisplayName(const QString& displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
}

} // namespace core
} // namespace warmonger
