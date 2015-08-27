#include <QFile>
#include <QJsonDocument>

#include "core/WorldSurface.h"
#include "core/JsonUtil.h"

using namespace warmonger::core;

static const QString category{"core"};

WorldSurface::WorldSurface(QObject *parent) :
    GameEntity(parent),
    tileSize(),
    gameMap(),
    miniMap()
{
}

QString WorldSurface::specification(const QString &objectName) const
{
    return "surfaces:" + objectName + ".wsd";
}

QSize WorldSurface::getTileSize() const
{
    return this->tileSize;
}

void WorldSurface::setTileSize(const QSize &tileSize)
{
    if (this->tileSize != tileSize)
    {
        this->tileSize = tileSize;
        emit tileSizeChanged();
    }
}

QMap<QString, QString> WorldSurface::getGameMap() const
{
    return this->gameMap;
}

void WorldSurface::setGameMap(const QMap<QString, QString> &gameMap)
{
    if (this->gameMap != gameMap)
    {
        this->gameMap = gameMap;
        emit gameMapChanged();
    }
}

QVariantMap WorldSurface::readGameMap() const
{
    return this->toQVariantMap(this->gameMap);
}

QMap<QString, QString> WorldSurface::getMiniMap() const
{
    return this->miniMap;
}

void WorldSurface::setMiniMap(const QMap<QString, QString> &miniMap)
{
    if (this->miniMap != miniMap)
    {
        this->miniMap = miniMap;
        emit miniMapChanged();
    }
}

QVariantMap WorldSurface::readMiniMap() const
{
    return this->toQVariantMap(this->miniMap);
}

void WorldSurface::dataFromJson(const QJsonObject &obj)
{
    GameEntity::dataFromJson(obj);
    this->tileSize = sizeFromJson(obj["tileSize"].toObject());
    this->gameMap = this->mapFromJson(obj["gameMap"].toObject());
    this->miniMap = this->mapFromJson(obj["miniMap"].toObject());
}

void WorldSurface::dataToJson(QJsonObject &obj) const
{
    GameEntity::dataToJson(obj);
    obj["tileSize"] = sizeToJson(this->tileSize);
    obj["gameMap"] = this->mapToJson(this->gameMap);
    obj["miniMap"] = this->mapToJson(this->miniMap);
}

QVariantMap WorldSurface::toQVariantMap(const QMap<QString, QString> &qmap) const
{
    QVariantMap vmap;
    QMap<QString, QString>::ConstIterator it;
    for (it = qmap.constBegin(); it != qmap.constEnd(); it++)
    {
        vmap.insert(it.key(), it.value());
    }

    return std::move(vmap);
}

QMap<QString, QString> WorldSurface::mapFromJson(const QJsonObject &obj) const
{
    QMap<QString, QString> map;
    QJsonObject::ConstIterator it;
    for (it = obj.constBegin(); it != obj.constEnd(); it++)
    {
        map.insert(it.key(), it.value().toString());
    }

    return std::move(map);
}

QJsonObject WorldSurface::mapToJson(const QMap<QString, QString> &map) const
{
    QJsonObject obj;
    QMap<QString, QString>::ConstIterator it;
    for (it = map.constBegin(); it != map.constEnd(); it++)
    {
        obj[it.key()] = it.value();
    }

    return std::move(obj);
}
