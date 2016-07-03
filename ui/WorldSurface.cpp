#include <cmath>

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QResource>

#include <ktar.h>

#include "core/Constants.h"
#include "core/Exception.h"
#include "io/Exception.h"
#include "ui/WorldSurface.h"

using namespace warmonger::ui;

static const QString loggerName{"core.WorldSurface"};

WorldSurface::WorldSurface(const QString& path, QObject *parent) :
    QObject(parent),
    path(path)
{
    KTar package(path);
    if (!package.open(QIODevice::ReadOnly))
    {
        throw io::FileIOError(
            "Failed to open surface package " + path + ". " + package.device()->errorString()
        );
    }

    const KArchiveDirectory *rootDir = package.directory();
    const QStringList entries = rootDir->entries();
    const auto& it = std::find_if(
        entries.cbegin(),
        entries.cend(),
        [](const QString& s){return s.endsWith("." + core::fileExtensions::SurfaceMetadata);}
    );

    if (it == entries.cend())
    {
        throw io::FileIOError("No metadata file found in surface package " + path);
    }

    const KArchiveEntry *headerEntry = rootDir->entry(*it);
    if (headerEntry->isDirectory())
    {
        throw io::FileIOError("Metadata file is not a file in surface package " + path);
    }

    const KArchiveFile *headerFile = dynamic_cast<const KArchiveFile *>(headerEntry);

    this->parseHeader(headerFile->data());
}

QString WorldSurface::getDisplayName() const
{
    return this->displayName;
}

void WorldSurface::setDisplayName(const QString &displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
}

QString WorldSurface::getDescription() const
{
    return this->description;
}

void WorldSurface::setDescription(const QString &description)
{
    if (this->description != description)
    {
        this->description = description;
        emit descriptionChanged();
    }
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

QColor WorldSurface::getNormalGridColor() const
{
    return this->normalGridColor;
}

void WorldSurface::setNormalGridColor(const QColor &color)
{
    if (this->normalGridColor != color)
    {
        this->normalGridColor = color;
        emit normalGridColorChanged();
    }
}

QColor WorldSurface::getFocusGridColor() const
{
    return this->focusGridColor;
}

void WorldSurface::setFocusGridColor(const QColor &color)
{
    if (this->focusGridColor != color)
    {
        this->focusGridColor = color;
        emit focusGridColorChanged();
    }
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

void WorldSurface::parseHeader(const QByteArray &header)
{
    /*
    if (!QResource::registerResource(path))
    {
        throw Exception("Failed to load resource from " + path);
    }

    QFile jfile("qrc::///default.wsd");
    jfile.open(QIODevice::ReadOnly);
    */

    QJsonParseError parseError;
    QJsonDocument jdoc = QJsonDocument::fromJson(header, &parseError);

    if (parseError.error != QJsonParseError::NoError)
    {
        throw io::JsonParseError(
            parseError.errorString() + " at " + parseError.offset
        );
    }

    QJsonObject jobj = jdoc.object();

    this->setObjectName(jobj["objectName"].toString());
    this->setDisplayName(jobj["displayName"].toString());
    this->setDescription(jobj["description"].toString());
    /*
    this->setTileWidth(jobj["tileWidth"].toInt());
    this->setTileHeight(jobj["tileHeight"].toInt());
    this->setNormalGridColor(QColor(jobj["normalGridColor"].toString()));
    this->setFocusGridColor(QColor(jobj["focusGridColor"].toString()));
    */
}
