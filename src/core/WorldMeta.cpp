#include <QFile>
#include <QJsonDocument>

#include "core/WorldMeta.h"

using namespace core;

WorldMeta::WorldMeta(QObject *parent) :
	QObject(parent)
{
}

QString WorldMeta::getObjectName() const
{
    return this->objectName();
}

QString WorldMeta::getDisplayName() const
{
    return this->displayName;
}

void WorldMeta::setDisplayName(const QString &displayName)
{
	this->displayName = displayName;
}

QString WorldMeta::getDescription() const
{
    return this->description;
}

void WorldMeta::setDescription(const QString &description)
{
	this->description = description;
}

WorldMeta * WorldMeta::newFromJson(const QJsonObject &obj, QObject *parent)
{
	WorldMeta *worldMeta = new WorldMeta(parent);
	worldMeta->fromJson(obj);
	return worldMeta;
}

void WorldMeta::fromJson(const QJsonObject &obj)
{
    this->setObjectName(obj["objectName"].toString());
    this->displayName = obj["displayName"].toString();
    this->description = obj["description"].toString();
}

QJsonObject WorldMeta::toJson() const
{
	QJsonObject obj;
	obj["objectName"] = this->objectName();
	obj["displayName"] = this->displayName;
	obj["description"] = this->description;

	return std::move(obj);
}
