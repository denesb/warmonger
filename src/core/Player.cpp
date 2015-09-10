#include "core/Player.h"
#include "core/Faction.h"
#include "core/Unit.h"
#include "core/Settlement.h"
#include "core/World.h"
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

QObject * Player::readFaction() const
{
    return this->faction;
}

void Player::writeFaction(QObject *faction)
{
    Faction *f = qobject_cast<Faction *>(faction);
    if (f == nullptr)
    {
        wError(category) << "faction is null or has wrong type";
        throw Exception(Exception::InvalidValue);
    }
    this->setFaction(f);
}

void Player::dataFromJson(const QJsonObject &obj)
{
    World *world = this->parent()->findChild<World *>(
        QString(),
        Qt::FindDirectChildrenOnly
    );

    if (world == nullptr)
    {
        wError("core.Player") << "world is null";
        throw Exception(Exception::NullPointer);
    }

    this->color = QColor(obj["color"].toString());
    this->goldBalance = obj["goldBalance"].toInt();
    this->faction = resolveReference<Faction>(
        obj["faction"].toString(), world
    );
}

void Player::dataToJson(QJsonObject &obj) const
{
    obj["color"] = this->color.name();
    obj["goldBalance"] = this->goldBalance;
    obj["faction"] = this->faction->objectName();
}
