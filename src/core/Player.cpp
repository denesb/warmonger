#include "core/Player.h"
#include "core/Faction.h"
#include "core/Unit.h"
#include "core/Settlement.h"
#include "core/World.h"
#include "core/JsonUtil.hpp"

using namespace warmonger::core;

Player::Player(QObject *parent) :
    GameObject(parent),
    displayName(),
    color(),
    goldBalance(0),
    faction(nullptr),
    units(),
    settlements()
{
}

Player::~Player()
{
}

QString Player::getDisplayName() const
{
    return this->displayName;
}

void Player::setDisplayName(const QString &displayName)
{
    this->displayName = displayName;
}

QColor Player::getColor() const
{
    return this->color;
}

void Player::setColor(const QColor &color)
{
    this->color = color;
}

int Player::getGoldBalance() const
{
    return this->goldBalance;
}

void Player::setGoldBalance(int goldBalance)
{
    this->goldBalance = goldBalance;
}

Faction * Player::getFaction() const
{
    return this->faction;
}

void Player::setFaction(Faction *faction)
{
    this->faction = faction;
}

QList<Unit *> Player::getUnits() const
{
    return this->units;
}

void Player::setUnits(const QList<Unit *> &units)
{
    this->units = units;
}

QList<Settlement *> Player::getSettlemets() const
{
    return this->settlements;
}

void Player::setSettlements(const QList<Settlement *> &settlements)
{
    this->settlements = settlements;
}

void Player::fromJson(const QJsonObject &obj)
{
    //TODO: error handling
    World *world = this->parent()->findChild<World *>(QString(), Qt::FindDirectChildrenOnly);

    this->setObjectName(obj["objectName"].toString());
    this->displayName = obj["displayName"].toString();
    this->color = QColor(obj["color"].toString());
    this->goldBalance = obj["goldBalance"].toInt();
    this->faction = world->findChild<Faction *>(obj["faction"].toString());
    this->units = newListFromJson<Unit>(obj["units"].toArray(), this);
    this->settlements = newListFromJson<Settlement>(obj["settlements"].toArray(), this);
}

QJsonObject Player::toJson() const
{
    QJsonObject obj;

    obj["objectName"] = this->objectName();
    obj["displayName"] = this->displayName;
    obj["color"] = this->color.name();
    obj["goldBalance"] = this->goldBalance;
    obj["faction"] = this->faction->objectName();
    obj["unit"] = listToJson<Unit>(this->units);
    obj["settlements"] = listToJson<Settlement>(this->settlements);

    return std::move(obj);
}
