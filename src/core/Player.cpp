#include "core/Faction.h"
#include "core/Map.h"
#include "core/Player.h"
#include "core/Settlement.h"
#include "core/World.h"
#include "core/Unit.h"
#include "core/Util.h"

using namespace warmonger::core;

static const QString category{"core"};

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
    if (this->color != color)
    {
        this->color = color;
        emit colorChanged();
    }
}

int Player::getGoldBalance() const
{
    return this->goldBalance;
}

void Player::setGoldBalance(int goldBalance)
{
    if (this->goldBalance != goldBalance)
    {
        this->goldBalance = goldBalance;
        emit goldBalanceChanged();
    }
}

Faction * Player::getFaction() const
{
    return this->faction;
}

void Player::setFaction(Faction *faction)
{
    if (this->faction != faction)
    {
        this->faction = faction;
        emit factionChanged();
    }
}

void Player::dataFromJson(const QJsonObject &obj)
{
    this->color = QColor(obj["color"].toString());
    this->goldBalance = obj["goldBalance"].toInt();

    Map *map = qobject_cast<Map *>(this->parent());
    this->faction = resolveReference<Faction>(
        obj["faction"].toString(),
        map->getWorld()
    );
}

void Player::dataToJson(QJsonObject &obj) const
{
    obj["color"] = this->color.name();
    obj["goldBalance"] = this->goldBalance;
    obj["faction"] = this->faction->objectName();
}
