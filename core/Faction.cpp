#include "core/Faction.h"
#include "core/Civilization.h"
#include "core/Settlement.h"
#include "core/Unit.h"
#include "core/World.h"

using namespace warmonger::core;

Faction::Faction(QObject* parent)
    : QObject(parent)
    , color()
    , goldBalance(0)
    , civilization(nullptr)
{
}

QString Faction::getDisplayName() const
{
    return this->displayName;
}

void Faction::setDisplayName(const QString& displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
}

QColor Faction::getColor() const
{
    return this->color;
}

void Faction::setColor(const QColor& color)
{
    if (this->color != color)
    {
        this->color = color;
        emit colorChanged();
    }
}

int Faction::getGoldBalance() const
{
    return this->goldBalance;
}

void Faction::setGoldBalance(int goldBalance)
{
    if (this->goldBalance != goldBalance)
    {
        this->goldBalance = goldBalance;
        emit goldBalanceChanged();
    }
}

Civilization* Faction::getCivilization() const
{
    return this->civilization;
}

void Faction::setCivilization(Civilization* civilization)
{
    if (this->civilization != civilization)
    {
        this->civilization = civilization;
        emit civilizationChanged();
    }
}
