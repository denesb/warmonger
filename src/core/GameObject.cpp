#include "core/GameObject.h"

using namespace warmonger::core;

GameObject::GameObject(QObject *parent) :
    QObject(parent),
	displayName("")
{
}

GameObject::~GameObject()
{
}

QString GameObject::getDisplayName() const
{
    return this->displayName;
}

void GameObject::setDisplayName(const QString &displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
}

void GameObject::fromJson(const QJsonObject &obj)
{
    this->setObjectName(obj["objectName"].toString());
    this->displayName = obj["displayName"].toString();

    this->dataFromJson(obj);
}

QJsonObject GameObject::toJson() const
{
	QJsonObject obj;
	obj["objectName"] = this->objectName();
	obj["displayName"] = this->displayName;

    this->dataToJson(obj);

	return std::move(obj);
}
