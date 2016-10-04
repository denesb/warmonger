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

void Settlement::setDisplayName(const QString& displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
}

void Settlement::setType(SettlementType* type)
{
    if (this->type != type)
    {
        this->type = type;
        emit typeChanged();
    }
}

void Settlement::setMapNode(MapNode* mapNode)
{
    if (this->mapNode != mapNode)
    {
        this->mapNode = mapNode;
        emit mapNodeChanged();
    }
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
