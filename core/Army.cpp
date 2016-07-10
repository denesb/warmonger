#include "core/Army.h"
#include "utils/QVariantUtils.h"

const QString loggerName{"core.Army"};

namespace warmonger {
namespace core {

Army::Army(QObject *parent) :
    QObject(parent),
    displayName("")
{
}

QString Army::getDisplayName() const
{
    return this->displayName;
}

void Army::setDisplayName(const QString &displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
}

MapNode * Army::getMapNode() const
{
    return this->mapNode;
}

void Army::setMapNode(MapNode *mapNode)
{
    if (this->mapNode != mapNode)
    {
        this->mapNode = mapNode;
        emit mapNodeChanged();
    }
}

Faction * Army::getOwner() const
{
    return this->owner;
}

void Army::setOwner(Faction *owner)
{
    if (this->owner != owner)
    {
        this->owner = owner;
        emit ownerChanged();
    }
}

std::vector<Unit *> Army::getUnits() const
{
    return this->units;
}

void Army::setUnits(const std::vector<Unit *> &units)
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
