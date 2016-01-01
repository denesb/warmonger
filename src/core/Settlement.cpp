#include "core/Map.h"
#include "core/Settlement.h"
#include "core/Util.h"

using namespace warmonger::core;

static const QString loggerName{"core.Settlement"};

Settlement::Settlement(QObject *parent) :
    GameObject(parent),
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

SettlementType * Settlement::getType() const
{
    return this->type;
}

void Settlement::setType(SettlementType *type)
{
    if (this->type != type)
    {
        QObject::disconnect(this->type, nullptr, this, nullptr);

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
        if (this->owner->getFaction() != nullptr)
            QObject::disconnect(
                this->owner->getFaction(),
                nullptr,
                this,
                nullptr
            );

        this->owner = owner;

        if (this->owner->getFaction() != nullptr)
            QObject::connect(
                this->owner->getFaction(),
                &Faction::recruitsChanged,
                this,
                &Settlement::recruitsChanged
            );

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
    return toQVariantList<UnitType *>(this->getRecruits());
}

void Settlement::dataFromJson(const QJsonObject &obj)
{
    Map *map = qobject_cast<Map *>(this->parent());
    World *world = map->getWorld();

    this->type = resolveReference<SettlementType>(
        obj["type"].toString(),
        world
    );
    this->mapNode = resolveReference<MapNode>(
        obj["mapNode"].toString(),
        this->parent()
    );
    this->owner = resolveReference<Player>(
        obj["owner"].toString(),
        this->parent()
    );

    QObject::connect(
        this->type,
        &SettlementType::recruitsChanged,
        this,
        &Settlement::recruitsChanged
    );

if (this->owner->getFaction() != nullptr)
        QObject::connect(
            this->owner->getFaction(),
            &Faction::recruitsChanged,
            this,
            &Settlement::recruitsChanged
        );
}

void Settlement::dataToJson(QJsonObject &obj) const
{
    obj["type"] = this->type->objectName();
    obj["mapNode"] = this->mapNode->objectName();
    obj["owner"] = this->owner->objectName();
}
