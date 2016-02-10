#include <QDir>

#include "core/QVariantUtil.h"
#include "core/QJsonUtil.h"
#include "core/WorldSurface.h"

using namespace warmonger::core;

const QString WorldSurface::fileExtension{"wsd"};

static QHash<QString, QImage> loadImages(
    const QMap<QString, QString> &imagePaths
);

static QHash<QString, QColor> createColors(
    const QMap<QString, QString> &colorNames
);

static const QString pathTemplate{"/surfaces/%1/%1.%2"};

static const QString loggerName{"core.WorldSurface"};

WorldSurface::WorldSurface(QObject *parent) :
    GameEntityPart(parent),
    tileWidth(0),
    tileHeight(0),
    images(),
    imagePaths(),
    colors(),
    colorNames()
{
}

QString WorldSurface::getEntityRelativePath(const QString &name) const
{
    return pathTemplate.arg(
        name,
        WorldSurface::fileExtension
    );
}

int WorldSurface::getTileWidth() const
{
    return this->tileWidth;
}

void WorldSurface::setTileWidth(int width)
{
    if (this->tileWidth != width)
    {
        this->tileWidth = width;
        emit tileWidthChanged();
        emit tileSizeChanged();
    }
}

int WorldSurface::getTileHeight() const
{
    return this->tileHeight;
}

void WorldSurface::setTileHeight(int height)
{
    if (this->tileHeight != height)
    {
        this->tileHeight = height;
        emit tileHeightChanged();
        emit tileSizeChanged();
    }

}

QSize WorldSurface::getTileSize() const
{
    return QSize(this->tileWidth, this->tileHeight);
}

void WorldSurface::setTileSize(const QSize &tileSize)
{
    if (this->tileWidth != tileSize.width() ||
        this->tileHeight != tileSize.height())
    {
        this->tileWidth = tileSize.width();
        this->tileHeight = tileSize.height();
        emit tileWidthChanged();
        emit tileHeightChanged();
        emit tileSizeChanged();
    }
}

QMap<QString, QString> WorldSurface::getImagePaths() const
{
    return this->imagePaths;
}

QString WorldSurface::getImagePath(const QString &key) const
{
    return this->imagePaths[key];
}

QVariantMap WorldSurface::readImagePaths() const
{
    return toQVariantMap(
        this->imagePaths,
        verbatim<QString>,
        QVariant::fromValue<QString>
    );
}

QImage WorldSurface::getImage(const QString &key) const
{
    return this->images[key];
}

QMap<QString, QString> WorldSurface::getColorNames() const
{
    return this->colorNames;
}

QString WorldSurface::getColorName(const QString &key) const
{
    return this->colorNames[key];
}

QVariantMap WorldSurface::readColorNames() const
{
    return toQVariantMap(
        this->colorNames,
        verbatim<QString>,
        QVariant::fromValue<QString>
    );
}

QColor WorldSurface::getColor(const QString &key) const
{
    return this->colors[key];
}

bool WorldSurface::hexContains(const QPoint &p) const
{
    const int x = p.x();
    const int y = p.y();
    const int w = this->tileWidth;
    const int h = this->tileHeight;

    if (x < 0 || x >= w || y < 0 || y >= h)
        return false;

    QRgb pixel = this->hexMask.pixel(x, y);

    return (pixel == 0xffffffff);
}

bool WorldSurface::hexContains(const QPointF &p) const
{
    const qreal x = p.x();
    const qreal y = p.y();
    const qreal w = static_cast<qreal>(this->tileWidth);
    const qreal h = static_cast<qreal>(this->tileHeight);

    if (x < 0.0 || x >= w || y < 0.0 || y >= h)
        return false;

    const int xc = static_cast<int>(ceil(x));
    const int yc = static_cast<int>(ceil(y));
    const int xf = static_cast<int>(floor(x));
    const int yf = static_cast<int>(floor(y));

    QRgb pixelc = this->hexMask.pixel(xc, yc);
    QRgb pixelf = this->hexMask.pixel(xf, yf);

    return (pixelc == 0xffffffff || pixelf == 0xffffffff);
}

void WorldSurface::dataFromJson(const QJsonObject &obj)
{
    this->hexMask.load(this->getPath() + QStringLiteral("/hexagon_mask.xpm"), "XPM");

    this->colorNames = fromQJsonObject<QMap<QString, QString>>(
        obj["colors"].toObject(),
        verbatim<QString>,
        qJsonValueToQString
    );
    this->imagePaths = fromQJsonObject<QMap<QString, QString>>(
        obj["images"].toObject(),
        verbatim<QString>,
        qJsonValueToQString
    );

    // Convert paths to absolute paths
    QMap<QString, QString>::Iterator it;
    QMap<QString, QString>::Iterator b = this->imagePaths.begin();
    QMap<QString, QString>::Iterator e = this->imagePaths.end();
    for (it = b; it != e; it++)
    {
        QString path = this->getPath() + "/" + it.value();
        it.value() = QDir::cleanPath(path);
    }

    this->tileWidth = obj["tileWidth"].toInt();
    this->tileHeight = obj["tileHeight"].toInt();

    this->colors = createColors(this->colorNames);
    this->images = loadImages(this->imagePaths);
}

void WorldSurface::dataToJson(QJsonObject &obj) const
{
    obj["tileWidth"] = this->tileWidth;
    obj["tileHeight"] = this->tileHeight;
    obj["images"] = toQJsonObject(
        this->imagePaths,
        verbatim<QString>,
        constructQJsonValue<QString>
    );
    obj["colors"] = toQJsonObject(
        this->colorNames,
        verbatim<QString>,
        constructQJsonValue<QString>
    );
}

QHash<QString, QColor> createColors(
    const QMap<QString, QString> &colorNames
)
{
    QHash<QString, QColor> colors;
    QMap<QString, QString>::ConstIterator it;
    for (it = colorNames.constBegin(); it != colorNames.constEnd(); it++)
    {
        colors.insert(it.key(), QColor(it.value()));
    }

    return colors;
}

QHash<QString, QImage> loadImages(
    const QMap<QString, QString> &imagePaths
)
{
    QHash<QString, QImage> images;
    QMap<QString, QString>::ConstIterator it;
    for (it = imagePaths.constBegin(); it != imagePaths.constEnd(); it++)
    {
        QImage image(it.value());
        images.insert(it.key(), image);
    }

    return images;
}
