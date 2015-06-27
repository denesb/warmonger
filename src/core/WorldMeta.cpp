#include <QFile>
#include <QJsonDocument>

#include "core/WorldMeta.h"

using namespace core;

WorldMeta::WorldMeta(
	const QString &name,
	const QString &displayName,
	const QString &description,
	const QString &path,
	QObject *parent
) :
	QObject(parent),
	name(name),
	displayName(displayName),
	description(description),
	path(path)
{
}

WorldMeta::WorldMeta(const QJsonObject &obj, QObject *parent) :
	QObject(parent)
{
    this->name = obj["name"].toString();
    this->displayName = obj["display_name"].toString();
    this->description = obj["description"].toString();
}

QString WorldMeta::getName() const
{
    return this->name;
}

QString WorldMeta::getDisplayName() const
{
    return this->displayName;
}

QString WorldMeta::getDescription() const
{
    return this->description;
}

QString WorldMeta::getPath() const
{
    return this->path;
}

QJsonObject toJson() const
{
}
