#include <algorithm>
#include <iterator>
#include <vector>

#include "core/Settlement.h"
#include "utils/QVariantUtils.h"

namespace warmonger {
namespace core {

Settlement::Settlement(QObject* parent)
    : QObject(parent)
    , type(nullptr)
    , mapNode(nullptr)
    , owner(nullptr)
{
}

Settlement::~Settlement()
{
}

QString Settlement::getDisplayName() const
{
    return this->displayName;
}

void Settlement::setDisplayName(const QString& displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
}

SettlementType* Settlement::getType() const
{
    return this->type;
}

void Settlement::setType(SettlementType* type)
{
    if (this->type != type)
    {
        this->type = type;
        emit typeChanged();
    }
}

MapNode* Settlement::getMapNode() const
{
    return this->mapNode;
}

void Settlement::setMapNode(MapNode* mapNode)
{
    if (this->mapNode != mapNode)
    {
        this->mapNode = mapNode;
        emit mapNodeChanged();
    }
}

Faction* Settlement::getOwner() const
{
    return this->owner;
}

void Settlement::setOwner(Faction* owner)
{
    if (this->owner != owner)
    {
        this->owner = owner;
        emit ownerChanged();
    }
}

} // namespace core
} // namespace warmonger
