#include "core/Player.h"
#include "core/Faction.h"
#include "core/Unit.h"
#include "core/Settlement.h"
#include "core/World.h"

using namespace warmonger::core;

Player::Player(QObject *parent) :
    GameObject(parent),
    color(),
    goldBalance(0),
    faction(nullptr)
{
}

Player::~Player()
{
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

const Faction * Player::getFaction() const
{
    return this->faction;
}

void Player::setFaction(const Faction *faction)
{
    this->faction = faction;
}

void Player::dataFromJson(const QJsonObject &obj)
{
    //TODO: error handling
    World *world = this->parent()->findChild<World *>(QString(), Qt::FindDirectChildrenOnly);

    this->color = QColor(obj["color"].toString());
    this->goldBalance = obj["goldBalance"].toInt();
    this->faction = world->findChild<Faction *>(obj["faction"].toString());
}

void Player::dataToJson(QJsonObject &obj) const
{
    obj["color"] = this->color.name();
    obj["goldBalance"] = this->goldBalance;
    obj["faction"] = this->faction->objectName();
}
