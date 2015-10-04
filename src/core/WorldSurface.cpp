#include <QFile>
#include <QJsonDocument>

#include "core/WorldSurface.h"
#include "core/JsonUtil.h"

using namespace warmonger::core;

static QHash<QString, QImage> loadImages(
    const QString &basePath,
    const QMap<QString, QString> &imagePaths
);

static const QString category{"core"};

WorldSurface::WorldSurface(QObject *parent) :
    GameEntityPart(parent),
    tileSize(),
    images(),
    imagePaths(),
    colors()
{
}

QString WorldSurface::fileExtension() const
{
    return QString("wsd");
}

QString WorldSurface::getPrefix() const
{
    return "Surface:";
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

QMap<QString, QString> WorldSurface::getImagePaths() const
{
    return this->imagePaths;
}

QString WorldSurface::getImagePath(const QString &name) const
{
    return this->imagePaths[name];
}

QVariantMap WorldSurface::readImagePaths() const
{
    return this->toQVariantMap(this->imagePaths);
}

QImage WorldSurface::getImage(const QString &name) const
{
    return this->images[name];
}

QMap<QString, QString> WorldSurface::getColors() const
{
    return this->colors;
}

QString WorldSurface::getColor(const QString &name) const
{
    return this->colors[name];
}

QVariantMap WorldSurface::readColors() const
{
    return this->toQVariantMap(this->colors);
}

bool WorldSurface::hexContains(const QPoint &p) const
{
    int x = p.x();
    int y = p.y();

    if (x < 0 || x >= this->tileSize.width() || y < 0 || y >= this->tileSize.height())
        return false;

    QRgb pixel = this->hexMask.pixel(x, y);
    if (pixel != 0xffffffff)
        return false;

    return true;
}

void WorldSurface::dataFromJson(const QJsonObject &obj)
{
    this->hexMask.load(this->getPath() + QStringLiteral("/hexagon_mask.xpm"), "XPM");

    this->tileSize = sizeFromJson(obj["tileSize"].toObject());
    this->imagePaths = this->mapFromJson(obj["images"].toObject());
    this->colors = this->mapFromJson(obj["colors"].toObject());

    this->images = loadImages(this->getPath(), this->imagePaths);
}

void WorldSurface::dataToJson(QJsonObject &obj) const
{
    obj["tileSize"] = sizeToJson(this->tileSize);
    obj["images"] = this->mapToJson(this->imagePaths);
    obj["colors"] = this->mapToJson(this->colors);
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

QHash<QString, QImage> loadImages(
    const QString &basePath,
    const QMap<QString, QString> &imagePaths
)
{
    QHash<QString, QImage> images;
    QMap<QString, QString>::ConstIterator it;
    for (it = imagePaths.constBegin(); it != imagePaths.constEnd(); it++)
    {
        const QString path = basePath + "/" + it.value();
        QImage image(path);
        images.insert(it.key(), image);
    }

    return std::move(images);
}
