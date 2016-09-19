#include "core/TerrainType.h"
#include "utils/Exception.h"

namespace warmonger {
namespace core {

TerrainType::TerrainType(QObject* parent)
    : QObject(parent)
{
}

QString TerrainType::getDisplayName() const
{
    if (this->displayName)
        return *this->displayName;
    else if (this->isHierarchyRoot())
        throw utils::ValueError("displayName is unset");
    else
        return this->getHierarchyParent()->getDisplayName();
}

void TerrainType::setDisplayName(const QString& displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
}

} // namespace core
} // namespace warmonger
