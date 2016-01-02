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
    tileSize(),
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
    const int w = this->tileSize.width();
    const int h = this->tileSize.height();

    if (x < 0 || x >= w || y < 0 || y >= h)
        return false;

    QRgb pixel = this->hexMask.pixel(x, y);

    return (pixel == 0xffffffff);
}

bool WorldSurface::hexContains(const QPointF &p) const
{
    const qreal x = p.x();
    const qreal y = p.y();
    const qreal w = static_cast<qreal>(this->tileSize.width());
    const qreal h = static_cast<qreal>(this->tileSize.height());

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

    this->tileSize = sizeFromJson(obj["tileSize"].toObject());

    this->colors = createColors(this->colorNames);
    this->images = loadImages(this->imagePaths);
}

void WorldSurface::dataToJson(QJsonObject &obj) const
{
    obj["tileSize"] = sizeToJson(this->tileSize);
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

    return std::move(colors);
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

    return std::move(images);
}
