#include "core/Banner.h"
#include "utils/QVariantUtils.h"

namespace warmonger {
namespace core {

Banner::Banner(QObject* parent)
    : QObject(parent)
{
}

void Banner::setDisplayName(const QString& displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
}

QVariantList Banner::readCivilizations() const
{
    return utils::toQVariantList(this->civilizations);
}

void Banner::setCivilizations(const std::vector<Civilization*>& civilizations)
{
    if (this->civilizations != civilizations)
    {
        this->civilizations = civilizations;
        emit civilizationsChanged();
    }
}

} // namespace core
} // namespace warmonger
