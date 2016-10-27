#include "core/Faction.h"
#include "core/Civilization.h"
#include "core/Settlement.h"
#include "core/Unit.h"
#include "core/World.h"

namespace warmonger {
namespace core {

Faction::Faction(QObject* parent)
    : QObject(parent)
    , civilization(nullptr)
{
}

void Faction::setDisplayName(const QString& displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
}

void Faction::setPrimaryColor(const QColor& primaryColor)
{
    if (this->primaryColor != primaryColor)
    {
        this->primaryColor = primaryColor;
        emit primaryColorChanged();
    }
}

void Faction::setSecondaryColor(const QColor& secondaryColor)
{
    if (this->secondaryColor != secondaryColor)
    {
        this->secondaryColor = secondaryColor;
        emit secondaryColorChanged();
    }
}

void Faction::setBanner(Banner* banner)
{
    if (this->banner != banner)
    {
        this->banner = banner;
        emit bannerChanged();
    }
}

void Faction::setCivilization(Civilization* civilization)
{
    if (this->civilization != civilization)
    {
        this->civilization = civilization;
        emit civilizationChanged();
    }
}

} // namespace core
} // namespace warmonger
