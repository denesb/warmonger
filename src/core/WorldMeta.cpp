#include <QFile>
#include <QJsonDocument>

#include "core/WorldMeta.h"

using namespace core;

WorldMeta::WorldMeta(const QString &path, QObject *parent) :
    QObject(parent),
    path(path)
{
    QFile metaFile(path + QStringLiteral("/meta.json"));

    if (!metaFile.open(QIODevice::ReadOnly))
    {
        //TODO: throw something
    }

    QByteArray metaJson = metaFile.readAll();
    QJsonDocument metaDoc(QJsonDocument::fromJson(metaJson));

    this->init(metaDoc.object());
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

void WorldMeta::init(const QJsonObject &metaObject)
{
    this->name = metaObject["name"].toString();
    this->displayName = metaObject["display_name"].toString();
    this->description = metaObject["description"].toString();
}
