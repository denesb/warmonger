#include "core/SettlementType.h"
#include "utils/QVariantUtils.h"

namespace warmonger {
namespace core {

SettlementType::SettlementType(QObject* parent)
    : QObject(parent)
{
}

QString SettlementType::getDisplayName() const
{
    if (this->displayName)
        return *this->displayName;
    else if (this->isHierarchyRoot())
        throw utils::ValueError("displayName is unset");
    else
        return this->getHierarchyParent()->getDisplayName();
}

void SettlementType::setDisplayName(const QString& displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
}

} // namespace core
} // namespace warmonger
