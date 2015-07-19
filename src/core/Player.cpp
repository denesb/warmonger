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
    World *world = this->parent()->findChild<World *>(QString(), Qt::FindDirectChildrenOnly);
    if (world == nullptr)
    {
        wError("core.Player") << "world is null";
        throw Exception(Exception::NullPointer);
    }

    this->color = QColor(obj["color"].toString());
    this->goldBalance = obj["goldBalance"].toInt();

    const QString factionName = obj["faction"].toString();
    this->faction = world->findChild<Faction *>(factionName);
    if (this->faction == nullptr)
    {
        wError("core.Player") << "Unable to resolve reference <Faction>" << factionName;
        throw Exception(Exception::UnresolvedReference, {"Faction", factionName});
    }
}

void Player::dataToJson(QJsonObject &obj) const
{
    obj["color"] = this->color.name();
    obj["goldBalance"] = this->goldBalance;
    obj["faction"] = this->faction->objectName();
}
