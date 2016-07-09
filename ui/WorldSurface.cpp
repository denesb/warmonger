#include <cmath>

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QResource>
#include <QSGTexture>

#include <ktar.h>

#include "log/LogStream.h"
#include "ui/WorldSurface.h"
#include "Constants.h"
#include "Exception.h"

using namespace warmonger;

static const QString loggerName{"core.WorldSurface"};

namespace warmonger {
namespace ui {

WorldSurface::WorldSurface(const QString& path, core::World *world, QQuickWindow *window, QObject *parent) :
    QObject(parent),
    path(path),
    world(world),
    window(window),
    isTextureSyncOn(window->isSceneGraphInitialized()),
    isTextureSyncPending(false)
{
    QObject::connect(window, &QQuickWindow::sceneGraphInitialized, this, &WorldSurface::turnTextureSyncOn);
    QObject::connect(window, &QQuickWindow::sceneGraphInvalidated, this, &WorldSurface::turnTextureSyncOff);

    KTar package(this->path);
    if (!package.open(QIODevice::ReadOnly))
    {
        throw IOError(
            "Failed to open surface package " + this->path + ". " + package.device()->errorString()
        );
    }

    const KArchiveDirectory *rootDir = package.directory();
    const QStringList entries = rootDir->entries();
    const auto& it = std::find_if(
        entries.cbegin(),
        entries.cend(),
        [](const QString& s){return s.endsWith("." + fileExtensions::surfaceMetadata);}
    );

    if (it == entries.cend())
    {
        throw IOError("No metadata file found in surface package " + this->path);
    }

    const KArchiveEntry *headerEntry = rootDir->entry(*it);
    if (headerEntry->isDirectory())
    {
        throw IOError("Metadata file is not a file in surface package " + this->path);
    }

    const KArchiveFile *headerFile = dynamic_cast<const KArchiveFile *>(headerEntry);

    this->parseHeader(headerFile->data());
}

WorldSurface::~WorldSurface()
{
    try
    {
        this->deactivate();
    }
    catch(...)
    {
        // ignore all exceptions
    }
}

QString WorldSurface::getPrefix() const
{
    static const QString prefix = resourcePaths::surface + "/";
    return prefix;
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

void WorldSurface::activate()
{
    KTar package(this->path);
    if (!package.open(QIODevice::ReadOnly))
    {
        throw IOError(
            "Failed to open surface package " + this->path + ". " + package.device()->errorString()
        );
    }
    const KArchiveDirectory *rootDir = package.directory();
    const QStringList entries = rootDir->entries();
    const QString rccEntryName = this->objectName() + "." + fileExtensions::qResourceData;

    if (std::find(entries.cbegin(), entries.cend(), rccEntryName) == entries.cend())
    {
        throw IOError("No rcc file found in surface package " + this->path);
    }

    const KArchiveEntry *rccEntry = rootDir->entry(rccEntryName);
    if (rccEntry->isDirectory())
    {
        throw IOError("rcc file is not a file in surface package " + this->path);
    }

    const KArchiveFile *rccFile = dynamic_cast<const KArchiveFile *>(rccEntry);
    this->resourceData = rccFile->data();

    const unsigned char * data = reinterpret_cast<const uchar *>(this->resourceData.data());
    if (!QResource::registerResource(data))
    {
        throw IOError("Failed to register  " + this->path);
    }

    QFile jfile(this->getPrefix() + this->objectName() + "." + fileExtensions::surfaceDefinition);
    if (!jfile.open(QIODevice::ReadOnly))
    {
        throw IOError("Failed to open surface definition from package " + this->path + ". " + jfile.errorString());
    }

    QJsonParseError parseError;
    const QJsonDocument jdoc = QJsonDocument::fromJson(jfile.readAll(), &parseError);

    if (parseError.error != QJsonParseError::NoError)
    {
        throw ValueError(
            "Error parsing surface definition file from surface package " + this->path + ". "
            + parseError.errorString() + " at " + parseError.offset
        );
    }

    const QJsonObject jobj = jdoc.object();

    this->setTileWidth(jobj["tileWidth"].toInt());
    this->setTileHeight(jobj["tileHeight"].toInt());
    this->setNormalGridColor(QColor(jobj["normalGridColor"].toString()));
    this->setFocusGridColor(QColor(jobj["focusGridColor"].toString()));

    if (!this->hexMask.load(this->getPrefix() + "hexagonMask.xpm"))
    {
        throw IOError("Hexagon mask not found in surface package " + this->path);
    }

    wInfo(loggerName) << "Succesfully activated surface " << this->objectName();

    if (this->isTextureSyncOn)
        this->uploadTextures();
    else
        this->isTextureSyncPending = true;
}

void WorldSurface::deactivate()
{
    const unsigned char * data = reinterpret_cast<const uchar *>(this->resourceData.data());
    if (!QResource::unregisterResource(data))
    {
        throw IOError("Failed to unregister  " + this->path);
    }

    wInfo(loggerName) << "Succesfully deactivated surface " << this->objectName();
}

void WorldSurface::turnTextureSyncOn()
{
    this->isTextureSyncOn = true;

    if (this->isTextureSyncPending)
    {
        this->uploadTextures();
        this->isTextureSyncPending = false;
    }
}

void WorldSurface::turnTextureSyncOff()
{
    this->isTextureSyncOn = false;
}

void WorldSurface::parseHeader(const QByteArray &header)
{
    QJsonParseError parseError;
    QJsonDocument jdoc = QJsonDocument::fromJson(header, &parseError);

    if (parseError.error != QJsonParseError::NoError)
    {
        throw ValueError(
            "Error parsing surface meta file from surface package " + this->path + ". "
            + parseError.errorString() + " at " + parseError.offset
        );
    }

    QJsonObject jobj = jdoc.object();

    this->setObjectName(jobj["objectName"].toString());
    this->setDisplayName(jobj["displayName"].toString());
    this->setDescription(jobj["description"].toString());
}

void WorldSurface::uploadTextures()
{
    this->textures.clear();

    const auto terrainTypes = this->world->getTerrainTypes();
    for (const auto& terrainType : terrainTypes)
    {
        this->uploadTexture(resourcePaths::terrainTypes, terrainType);
    }

    const auto settlementTypes = this->world->getTerrainTypes();
    for (const auto& settlementType : settlementTypes)
    {
        this->uploadTexture(resourcePaths::settlementTypes, settlementType);
    }

    const auto unitTypes = this->world->getTerrainTypes();
    for (const auto& unitType : unitTypes)
    {
        this->uploadTexture(resourcePaths::unitTypes, unitType);
    }
}

void WorldSurface::uploadTexture(const QString &pathPrefix, const QObject *object)
{
    const QString path = pathPrefix + "/" + object->objectName() + "." + resourcePaths::fileExtension;
    QImage image(path);

    if (image.isNull())
    {
        wWarning(loggerName) << "Cannot find texture for " << object;
        return;
    }

    this->textures[path] = std::unique_ptr<QSGTexture>(this->window->createTextureFromImage(image));
}

} // namespace ui
} // namespace warmonger
