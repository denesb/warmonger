#include "core/WorldItem.h"

using namespace warmonger::core;

WorldItem::WorldItem(QObject *parent) :
	QObject(parent),
	displayName("")
{
}

WorldItem::~WorldItem()
{
}

QString WorldItem::getDisplayName() const
{
    return this->displayName;
}

void WorldItem::setDisplayName(const QString &displayName)
{
	this->displayName = displayName;
}

bool WorldItem::isRootWorldItem()
{
	QObject *parent = this->parent();
	bool isRootWorldItem = false;

	if (parent)
	{
		isRootWorldItem = qobject_cast<WorldItem *>(parent);
	}
	else
	{
		isRootWorldItem = true;
	}

	return isRootWorldItem;
}

void WorldItem::fromJson(const QJsonObject &obj)
{
    this->setObjectName(obj["objectName"].toString());
    this->displayName = obj["displayName"].toString();
}

QJsonObject WorldItem::toJson() const
{
	QJsonObject obj;
	obj["objectName"] = this->objectName();
	obj["displayName"] = this->displayName;

	return std::move(obj);
}
