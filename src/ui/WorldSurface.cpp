/**
 * Copyright (C) 2015-2017 Botond Dénes
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <cmath>
#include <set>
#include <unordered_map>

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QHash>
#include <QJsonDocument>
#include <QJsonObject>
#include <QQuickWindow>
#include <QResource>
#include <QSGTexture>

#include <ktar.h>

#include "ui/WorldSurface.h"
#include "utils/Constants.h"
#include "utils/Exception.h"
#include "utils/Hash.h"
#include "utils/Logging.h"
#include "utils/PathBuilder.h"

namespace std {

template <>
struct hash<pair<QString, QQuickWindow*>>
{
    std::size_t operator()(const pair<QString, QQuickWindow*>& key) const
    {
        // FIXME make it not collide across windows
        return qHash(key.first);
    }
};
}

namespace warmonger {
namespace ui {

namespace {

const QString fileExtension{"png"};
const QString hexagonMask{"hexagonMask.png"};

} // anonymus namespace

static bool hasAllMandatoryImages(const WorldSurface& surface);
static QSGTexture* createTexture(const QImage& image, QQuickWindow* window);

const std::vector<QString> staticImages{};

class WorldSurface::Storage
{
public:
    struct Header
    {
        QString name;
        QString description;
    };

    struct Body
    {
        int tileWidth;
        int tileHeight;
    };

    Storage(QString path)
        : path(std::move(path))
    {
    }

    virtual Header load() = 0;
    virtual Body activate() = 0;
    virtual void deactivate() = 0;

    QSGTexture* getTexture(const QString& path, QQuickWindow* window) const;
    QImage getImage(const QString& path) const;
    virtual QUrl getImageUrl(const QString& path) const = 0;

    const QString& getPath() const
    {
        return this->path;
    }

    const QString& getName() const
    {
        return this->name;
    }

protected:
    Header parseHeader(const QByteArray& header);
    void cache(const QString& path, const QImage& image) const
    {
        this->imageCache.insert({path, image});
    }
    QImage lookup(const QString& path) const
    {
        auto it = this->imageCache.find(path);
        return it == this->imageCache.end() ? QImage() : it->second;
    }

    virtual QImage loadImage(const QString& path) const = 0;

private:
    QString path;
    QString name;
    mutable std::unordered_map<QString, QImage> imageCache;
    mutable std::unordered_map<std::pair<QString, QQuickWindow*>,
        std::unique_ptr<QSGTexture, utils::DelayedQObjectDeleter>>
        textures;
};

class DirectoryStorage : public WorldSurface::Storage
{
public:
    DirectoryStorage(QString path);

    Header load() override;
    Body activate() override;
    void deactivate() override;
    QUrl getImageUrl(const QString& path) const override;

private:
    QImage loadImage(const QString& path) const override;
};

class ArchiveStorage : public WorldSurface::Storage
{
    // TODO maybe make dynamic so that multiple surfaces can be loaded
    // side-by-side
    static const QString rootPath;

public:
    using WorldSurface::Storage::Storage;

    Header load() override;
    Body activate() override;
    void deactivate() override;
    QUrl getImageUrl(const QString& path) const override;

private:
    QImage loadImage(const QString& path) const override;

    QByteArray resourceData;
};

const QString ArchiveStorage::rootPath{":/surface"};

WorldSurface::WorldSurface(QString path, core::World* world, QObject* parent)
    : QObject(parent)
    , path(std::move(path))
    , world(world)
{
    if (this->path.endsWith(utils::fileExtensions::surfacePackage))
        this->storage = std::make_unique<ArchiveStorage>(this->path);
    else
        this->storage = std::make_unique<DirectoryStorage>(this->path);

    this->storage->load();
}

WorldSurface::~WorldSurface()
{
    try
    {
        this->deactivate();
    }
    catch (...)
    {
        // ignore all exceptions
    }
}

bool WorldSurface::hexContains(const QPoint& p) const
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

bool WorldSurface::hexContains(const QPointF& p) const
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
    auto body = storage->activate();

    this->tileWidth = body.tileWidth;
    this->tileHeight = body.tileHeight;
    this->hexMask = storage->getImage(hexagonMask);

    emit tileWidthChanged();
    emit tileHeightChanged();
    emit tileSizeChanged();
}

void WorldSurface::deactivate()
{
    this->storage->deactivate();
}

QSGTexture* WorldSurface::getTexture(const QString& path, QQuickWindow* window) const
{
    return storage->getTexture(path, window);
}

QImage WorldSurface::getImage(const QString& path) const
{
    return storage->getImage(path);
}

QUrl WorldSurface::getImageUrl(const QString& path) const
{
    return storage->getImageUrl(path);
}

QString WorldSurface::getBannerImagePath(const core::Banner* const banner) const
{
    return QStringLiteral("banners") / banner->getName() + "." + fileExtension;
}

QString findWorldSurface(const QString& surface, const QString& worldPath)
{
    if (QFile(surface).exists())
        return QFileInfo(surface).canonicalFilePath();

    QFileInfo worldFile(worldPath);

    auto surfaceDir = QDir(worldFile.canonicalPath() / utils::paths::surfaces / surface);

    if (!surfaceDir.exists())
        return {};

    for (const auto& suffix : {utils::fileExtensions::surfaceDefinition, utils::fileExtensions::surfacePackage})
    {
        const auto file = surface + "." + suffix;
        if (surfaceDir.exists(file))
            return file;
    }

    return {};
}

bool isWorldSurfaceSane(const QString& path, core::World* world)
{
    std::unique_ptr<WorldSurface> worldSurface;

    try
    {
        worldSurface = std::make_unique<WorldSurface>(path, world);
    }
    catch (std::exception& e)
    {
        wError << "Caught exception while trying to read world-surface: " << e.what();
        return false;
    }

    try
    {
        worldSurface->activate();
    }
    catch (std::exception& e)
    {
        wError << "Caught exception while trying to activate world-surface: " << e.what();
        return false;
    }

    return hasAllMandatoryImages(*worldSurface);
}

// Local functions

QSGTexture* WorldSurface::Storage::getTexture(const QString& path, QQuickWindow* window) const
{
    QSGTexture* texture{nullptr};

    const auto textureKey = std::make_pair(path, window);
    const auto it = this->textures.find(textureKey);

    if (it == this->textures.end())
    {
        texture = createTexture(this->getImage(path), window);

        if (texture != nullptr)
        {
            this->textures.emplace(textureKey, std::unique_ptr<QSGTexture, utils::DelayedQObjectDeleter>(texture));
            wDebug << "Created texture for " << path;
        }
    }
    else
    {
        texture = it->second.get();
    }

    return texture;
}

QImage WorldSurface::Storage::getImage(const QString& path) const
{
    auto image = this->lookup(path);

    if (image.isNull())
    {
        return loadImage(path);
    }

    return image;
}

WorldSurface::Storage::Header WorldSurface::Storage::parseHeader(const QByteArray& rawHeader)
{
    QJsonParseError parseError;
    QJsonDocument jdoc = QJsonDocument::fromJson(rawHeader, &parseError);

    if (parseError.error != QJsonParseError::NoError)
    {
        throw utils::ValueError("Error parsing surface meta file from surface package " + this->path + ". " +
            parseError.errorString() + " at " + parseError.offset);
    }

    QJsonObject jobj = jdoc.object();

    WorldSurface::Storage::Header header;

    header.name = jobj["name"].toString();
    header.description = jobj["description"].toString();

    this->name = header.name;

    return header;
}

DirectoryStorage::DirectoryStorage(QString path)
    : WorldSurface::Storage(QFileInfo(path).canonicalPath())
{
}

WorldSurface::Storage::Header DirectoryStorage::load()
{
    QDir surfaceDir(this->getPath());

    const auto filter = QDir::Files;
    const auto entries = surfaceDir.entryList({"*." + utils::fileExtensions::surfaceDefinition}, filter);

    if (entries.isEmpty())
        throw utils::IOError("No definition file found");

    QFile definitionFile(surfaceDir / entries.front());
    if (!definitionFile.open(QIODevice::ReadOnly))
    {
        throw utils::IOError(fmt::format(
            "Failed to open definition file `{}': {}", definitionFile.fileName(), definitionFile.errorString()));
    }
    return parseHeader(definitionFile.readAll());
}

WorldSurface::Storage::Body DirectoryStorage::activate()
{
    return Body();
}

void DirectoryStorage::deactivate()
{
}

QImage DirectoryStorage::loadImage(const QString& path) const
{
    return QImage(this->getPath() / path);
}

QUrl DirectoryStorage::getImageUrl(const QString& path) const
{
    return QUrl::fromLocalFile(this->getPath() / path);
}

WorldSurface::Storage::Header ArchiveStorage::load()
{
    KTar package(this->getPath());
    if (!package.open(QIODevice::ReadOnly))
    {
        throw utils::IOError(
            "Failed to open surface package " + this->getPath() + ". " + package.device()->errorString());
    }

    const KArchiveDirectory* rootDir = package.directory();
    const QStringList entries = rootDir->entries();
    const auto& it = std::find_if(entries.cbegin(), entries.cend(), [](const QString& s) {
        return s.endsWith("." + utils::fileExtensions::surfaceMetadata);
    });

    if (it == entries.cend())
    {
        throw utils::IOError("No metadata file found in surface package " + this->getPath());
    }

    const KArchiveEntry* headerEntry = rootDir->entry(*it);
    if (headerEntry->isDirectory())
    {
        throw utils::IOError("Metadata file is not a file in surface package " + this->getPath());
    }

    const KArchiveFile* headerFile = dynamic_cast<const KArchiveFile*>(headerEntry);

    return this->parseHeader(headerFile->data());
}

WorldSurface::Storage::Body ArchiveStorage::activate()
{
    KTar package(this->getPath());
    if (!package.open(QIODevice::ReadOnly))
    {
        throw utils::IOError(
            "Failed to open surface package " + this->getPath() + ": " + package.device()->errorString());
    }
    const KArchiveDirectory* rootDir = package.directory();
    const QStringList entries = rootDir->entries();
    const QString rccEntryName = this->getName() + "." + utils::fileExtensions::qResourceData;

    if (std::find(entries.cbegin(), entries.cend(), rccEntryName) == entries.cend())
    {
        throw utils::IOError("No rcc file found in surface package " + this->getPath());
    }

    const KArchiveEntry* rccEntry = rootDir->entry(rccEntryName);
    if (rccEntry->isDirectory())
    {
        throw utils::IOError("rcc file is not a file in surface package " + this->getPath());
    }

    const KArchiveFile* rccFile = dynamic_cast<const KArchiveFile*>(rccEntry);
    this->resourceData = rccFile->data();

    const unsigned char* data = reinterpret_cast<const uchar*>(this->resourceData.data());
    if (!QResource::registerResource(data))
    {
        throw utils::IOError("Failed to register  " + this->getPath());
    }

    QFile jfile(ArchiveStorage::rootPath / this->getName() + "." + utils::fileExtensions::surfaceDefinition);
    if (!jfile.open(QIODevice::ReadOnly))
    {
        throw utils::IOError(
            "Failed to open surface definition from package " + this->getPath() + ": " + jfile.errorString());
    }

    QJsonParseError parseError;
    const QJsonDocument jdoc = QJsonDocument::fromJson(jfile.readAll(), &parseError);

    if (parseError.error != QJsonParseError::NoError)
    {
        throw utils::ValueError("Error parsing surface definition file from surface package " + this->getPath() + ". " +
            parseError.errorString() + " at " + parseError.offset);
    }

    const QJsonObject jobj = jdoc.object();

    Body body;

    body.tileWidth = jobj["tileWidth"].toInt();
    body.tileHeight = jobj["tileHeight"].toInt();

    return body;
}

void ArchiveStorage::deactivate()
{
    const unsigned char* data = reinterpret_cast<const uchar*>(this->resourceData.data());
    if (!QResource::unregisterResource(data))
    {
        throw utils::IOError("Failed to unregister  " + this->getPath());
    }

    wInfo << "Succesfully deactivated surface " << this->getName();
}

QImage ArchiveStorage::loadImage(const QString& path) const
{
    return QImage(QStringLiteral(":") / path);
}

QUrl ArchiveStorage::getImageUrl(const QString& path) const
{
    return QUrl::fromLocalFile(QStringLiteral("qrc://") + path);
}

static bool hasAllMandatoryImages(const WorldSurface& surface)
{
    const auto isImageMissing = [&](const QString& path) { return surface.getImage(path).isNull(); };

    const auto isBannerImageMissing = [&](
        const core::Banner* b) { return isImageMissing(surface.getBannerImagePath(b)); };

    const core::World* world = surface.getWorld();

    const auto& banner = world->getBanners();
    if (std::any_of(banner.cbegin(), banner.cend(), isBannerImageMissing))
    {
        return false;
    }

    if (std::any_of(staticImages.cbegin(), staticImages.cend(), isImageMissing))
    {
        return false;
    }

    return true;
}

static QSGTexture* createTexture(const QImage& image, QQuickWindow* window)
{
    if (image.isNull())
    {
        return nullptr;
    }

    return window->createTextureFromImage(image);
}

} // namespace ui
} // namespace warmonger
