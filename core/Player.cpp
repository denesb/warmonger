#include "core/Faction.h"
#include "core/Map.h"
#include "core/Player.h"
#include "core/Settlement.h"
#include "core/World.h"
#include "core/Unit.h"

using namespace warmonger::core;

static const QString loggerName{"core.Player"};

Player::Player(QObject *parent) :
    QObject(parent),
    color(),
    goldBalance(0),
    faction(nullptr)
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
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
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
