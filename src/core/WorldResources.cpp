#include <QFile>
#include <QJsonDocument>

#include "core/WorldResources.h"
#include "core/World.h"
#include "core/Exception.h"
#include "log/LogStream.h"

using namespace warmonger::core;

WorldResources::WorldResources(QObject *parent) :
    QObject(parent),
    basePath(),
    resourcePaths()
{
    World *world = qobject_cast<World *>(this->parent());
    this->basePath = world->getPath() + QStringLiteral("/resources/");
}

QString WorldResources::getBasePath() const
{
    return this->basePath;
}

QString WorldResources::getPath(const QString &resourceName) const
{
    return this->basePath + this->resourcePaths[resourceName];
}

QVariantMap WorldResources::readPaths() const
{
    return std::move(this->toQVariantMap(this->resourcePaths));
}

void WorldResources::loadFromJsonFile()
{
    const QString path(this->basePath + "resources.json");
    QFile jsonFile(path);

    if (!jsonFile.open(QIODevice::ReadOnly))
    {
        Exception e(Exception::FileOpenFailed, {path});
        wError("core.WorldResources") << e.getMessage();
        throw e;
    }

    QByteArray jsonData = jsonFile.readAll();
    jsonFile.close();

    QJsonParseError parseError;
    QJsonDocument doc(QJsonDocument::fromJson(jsonData, &parseError));

    if (parseError.error != QJsonParseError::NoError)
    {
        Exception e(Exception::JsonParse, {path, parseError.errorString()});
        wError("core.WorldResources") << e.getMessage();
        throw e;
    }

    wInfo("core.WorldResources") << "Loaded Json document from " << path;

    this->fromJson(doc.object());
}
void WorldResources::fromJson(const QJsonObject &obj)
{
    this->resourcePaths = this->mapFromJson(obj);
}

QJsonObject WorldResources::toJson() const
{
    QJsonObject obj;

    obj = this->mapToJson(this->resourcePaths);

    return std::move(obj);
}

QVariantMap WorldResources::toQVariantMap(const QMap<QString, QString> &qmap) const
{
    QVariantMap vmap;
    QMap<QString, QString>::ConstIterator it;
    for (it = qmap.constBegin(); it != qmap.constEnd(); it++)
    {
        const QString path(this->basePath + it.value());
        vmap.insert(it.key(), QVariant(path));
    }

    return std::move(vmap);
}

QMap<QString, QString> WorldResources::mapFromJson(const QJsonObject &obj) const
{
    QMap<QString, QString> map;
    QJsonObject::ConstIterator it;
    for (it = obj.constBegin(); it != obj.constEnd(); it++)
    {
        map.insert(it.key(), it.value().toString());
    }

    return std::move(map);
}

QJsonObject WorldResources::mapToJson(const QMap<QString, QString> &map) const
{
    QJsonObject obj;
    QMap<QString, QString>::ConstIterator it;
    for (it = map.constBegin(); it != map.constEnd(); it++)
    {
        obj[it.key()] = it.value();
    }

    return std::move(obj);
}
