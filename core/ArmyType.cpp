#include "core/ArmyType.h"
#include "utils/Exception.h"

namespace warmonger {
namespace core {

ArmyType::ArmyType(QObject* parent)
    : QObject(parent)
{
}

QString ArmyType::getDisplayName() const
{
    if (this->displayName)
        return *this->displayName;
    else if (this->isHierarchyRoot())
        throw utils::ValueError("displayName is unset");
    else
        return this->getHierarchyParent()->getDisplayName();
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
