#include "core/QVariantUtil.h"
#include "core/Settlement.h"
#include "core/UnitType.h"

using namespace warmonger::core;

static const QString loggerName{"core.Settlement"};

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

Player * Settlement::getOwner() const
{
    return this->owner;
}

void Settlement::setOwner(Player *owner)
{
    if (this->owner != owner)
    {
        if (this->owner != nullptr)
        {
            this->owner->getFaction()->disconnect(this);
        }

        this->owner = owner;

        if (this->owner != nullptr)
        {
            QObject::connect(
                this->owner->getFaction(),
                &Faction::recruitsChanged,
                this,
                &Settlement::recruitsChanged
            );
        }

        emit ownerChanged();
    }
}

QList<UnitType *> Settlement::getRecruits() const
{
    QList<UnitType *> recruits;

    if (this->owner != nullptr)
        recruits << this->owner->getFaction()->getRecruitsFor(this->type);

    recruits << this->type->getRecruits();

    return recruits;
}

QVariantList Settlement::readRecruits() const
{
    return toQVariantList(this->getRecruits());
}
