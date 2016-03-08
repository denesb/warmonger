#include "core/UnitLevel.h"

using namespace warmonger::core;

static const QString loggerName{"core.UnitLevel"};

UnitLevel::UnitLevel(QObject *parent) :
    QObject(parent),
    experiencePoints(0),
    index(0)
{
}

UnitLevel::~UnitLevel()
{
}

QString UnitLevel::getDisplayName() const
{
    return this->displayName;
}

void UnitLevel::setDisplayName(const QString &displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
}

int UnitLevel::getExperiencePoints() const
{
    return this->experiencePoints;
}

void UnitLevel::setExperiencePoints(int experiencePoints)
{
    if (this->experiencePoints != experiencePoints)
    {
        this->experiencePoints = experiencePoints;
        emit experiencePointsChanged();
    }
}

int UnitLevel::getIndex() const
{
    return this->index;
}

void UnitLevel::setIndex(int index)
{
    if (this->index != index)
    {
        this->index = index;
        emit indexChanged();
    }
}
