#include <iterator>
#include <algorithm>
#include <vector>

#include "core/Settlement.h"
#include "core/UnitType.h"
#include "utils/QVariantUtils.h"

static const QString loggerName{"core.Settlement"};

namespace warmonger {
namespace core {

Settlement::Settlement(QObject *parent) :
    QObject(parent),
    type(nullptr),
    mapNode(nullptr),
    owner(nullptr)
{
    QObject::connect(
        this,
        &Settlement::typeChanged,
        this,
        &Settlement::recruitsChanged
    );
    QObject::connect(
        this,
        &Settlement::ownerChanged,
        this,
        &Settlement::recruitsChanged
    );
}

Settlement::~Settlement()
{
}

QString Settlement::getDisplayName() const
{
    return this->displayName;
}

void Settlement::setDisplayName(const QString &displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
}

SettlementType * Settlement::getType() const
{
    return this->type;
}

void Settlement::setType(SettlementType *type)
{
    if (this->type != type)
    {
        this->type->disconnect(this);

        this->type = type;

        QObject::connect(
            this->type,
            &SettlementType::recruitsChanged,
            this,
            &Settlement::recruitsChanged
        );

        emit typeChanged();
    }
}

MapNode * Settlement::getMapNode() const
{
    return this->mapNode;
}

void Settlement::setMapNode(MapNode *mapNode)
{
    if (this->mapNode != mapNode)
    {
        this->mapNode = mapNode;
        emit mapNodeChanged();
    }
}

Faction * Settlement::getOwner() const
{
    return this->owner;
}

void Settlement::setOwner(Faction *owner)
{
    if (this->owner != owner)
    {
        if (this->owner != nullptr)
        {
            this->owner->getCivilization()->disconnect(this);
        }

        this->owner = owner;

        if (this->owner != nullptr)
        {
            QObject::connect(
                this->owner->getCivilization(),
                &Civilization::recruitsChanged,
                this,
                &Settlement::recruitsChanged
            );
        }

        emit ownerChanged();
    }
}

std::vector<UnitType *> Settlement::getRecruits() const
{
    std::vector<UnitType *> recruits;

    if (this->owner != nullptr)
    {
        const std::vector<UnitType *> crs(this->owner->getCivilization()->getRecruitsFor(this->type));
        std::copy(crs.cbegin(), crs.cend(), std::back_inserter(recruits));
    }

    const std::vector<UnitType *> srs{this->type->getRecruits()};
    std::copy(srs.cbegin(), srs.cend(), std::back_inserter(recruits));

    return recruits;
}

QVariantList Settlement::readRecruits() const
{
    return utils::toQVariantList(this->getRecruits());
}

} // namespace core
} // namespace warmonger
