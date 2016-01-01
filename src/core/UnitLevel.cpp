#include "core/UnitLevel.h"
#include "core/JsonUtil.h"

using namespace warmonger::core;

static const QString loggerName{"core.UnitLevel"};

UnitLevel::UnitLevel(QObject *parent) :
    GameObject(parent),
    experiencePoints(0),
    index(0)
{
}

UnitLevel::~UnitLevel()
{
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

void UnitLevel::dataFromJson(const QJsonObject &obj)
{
    this->experiencePoints = obj["experiencePoints"].toInt();
    this->index = obj["index"].toInt();
}

void UnitLevel::dataToJson(QJsonObject &obj) const
{
    obj["experiencePoints"] = this->experiencePoints;
    obj["index"] = this->index;
}
