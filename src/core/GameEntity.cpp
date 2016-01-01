#include <QFileInfo>

#include "core/GameEntity.h"
#include "core/GameObject.h"
#include "core/JsonUtil.h"

using namespace warmonger::core;

static const QString loggerName("core.GameEntity");

GameEntity::GameEntity(QObject *parent) :
    QObject(parent),
    displayName(""),
    description("")
{
}

GameEntity::~GameEntity()
{
}

QString GameEntity::getDescription() const
{
    return this->description;
}

void GameEntity::setDescription(const QString &description)
{
    if (this->description != description)
    {
        this->description = description;
        emit descriptionChanged();
    }
}

QString GameEntity::getDisplayName() const
{
    return this->displayName;
}

void GameEntity::setDisplayName(const QString &displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
}

void GameEntity::fromJson(const QJsonObject &obj)
{
    this->setObjectName(obj["objectName"].toString());
    this->displayName = obj["displayName"].toString();
    this->description = obj["description"].toString();

    this->dataFromJson(obj);
}

QJsonObject GameEntity::toJson() const
{
	QJsonObject obj;
	obj["objectName"] = this->objectName();
	obj["displayName"] = this->displayName;
	obj["description"] = this->description;

    this->dataToJson(obj);

	return std::move(obj);
}
