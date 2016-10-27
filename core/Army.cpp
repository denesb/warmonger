#include "core/Army.h"
#include "utils/QVariantUtils.h"

namespace warmonger {
namespace core {

Army::Army(QObject* parent)
    : QObject(parent)
    , displayName("")
    , type(nullptr)
    , mapNode(nullptr)
    , owner(nullptr)
{
}

void Army::setDisplayName(const QString& displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
}

void Army::setType(ArmyType* type)
{
    if (this->type != type)
    {
        this->type = type;
        emit typeChanged();
    }
}

void Army::setMapNode(MapNode* mapNode)
{
    if (this->mapNode != mapNode)
    {
        this->mapNode = mapNode;
        emit mapNodeChanged();
    }
}

void Army::setOwner(Faction* owner)
{
    if (this->owner != owner)
    {
        this->owner = owner;
        emit ownerChanged();
    }
}

void Army::setUnits(const std::vector<Unit*>& units)
{
    if (this->units != units)
    {
        this->units = units;
        emit unitsChanged();
    }
}

QVariantList Army::readUnits() const
{
    return utils::toQVariantList(this->units);
}

} // namespace core
} // namespace warmonger
