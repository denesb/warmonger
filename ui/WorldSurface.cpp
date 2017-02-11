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

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QQuickWindow>
#include <QResource>
#include <QSGTexture>

#include <ktar.h>

#include "ui/WorldSurface.h"
#include "utils/Constants.h"
#include "utils/Exception.h"
#include "utils/Logging.h"
#include "utils/Utils.h"

namespace warmonger {
namespace ui {

namespace {

const QString resourceSchema{"qrc"};
const QString fileExtension{"png"};
const QString prefix{":"};

namespace path {

const QString surface{utils::makePath(prefix, "surface")};

const QString hexagonMask{utils::makePath(surface, QString("hexagonMask.png"))};

const QString mapEditor{utils::makePath(surface, "MapEditor")};

} // namespace path
} // anonymus namespace

static QString objectPath(const QObject* const object);
static QString imagePath(WorldSurface::Image image);
static void checkMissingImages(const WorldSurface* const surface);
static QSGTexture* createTexture(const QImage& image, QQuickWindow* window);

const std::vector<WorldSurface::Image> staticImages{};

const std::set<QString> visualClasses{"warmonger::core::ArmyType",
    "warmonger::core::Banner",
    "warmonger::core::SettlementType",
    "warmonger::core::TerrainType",
    "warmonger::core::UnitType"};

WorldSurface::WorldSurface(const QString& path, core::World* world, QObject* parent)
    : QObject(parent)
    , path(path)
    , world(world)
{
    KTar package(this->path);
    if (!package.open(QIODevice::ReadOnly))
    {
        throw utils::IOError("Failed to open surface package " + this->path + ". " + package.device()->errorString());
    }

    const KArchiveDirectory* rootDir = package.directory();
    const QStringList entries = rootDir->entries();
    const auto& it = std::find_if(entries.cbegin(), entries.cend(), [](const QString& s) {
        return s.endsWith("." + utils::fileExtensions::surfaceMetadata);
    });

    if (it == entries.cend())
    {
        throw utils::IOError("No metadata file found in surface package " + this->path);
    }

    const KArchiveEntry* headerEntry = rootDir->entry(*it);
    if (headerEntry->isDirectory())
    {
        throw utils::IOError("Metadata file is not a file in surface package " + this->path);
    }

    const KArchiveFile* headerFile = dynamic_cast<const KArchiveFile*>(headerEntry);

    this->parseHeader(headerFile->data());
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
    KTar package(this->path);
    if (!package.open(QIODevice::ReadOnly))
    {
        throw utils::IOError("Failed to open surface package " + this->path + ": " + package.device()->errorString());
    }
    const KArchiveDirectory* rootDir = package.directory();
    const QStringList entries = rootDir->entries();
    const QString rccEntryName = this->objectName() + "." + utils::fileExtensions::qResourceData;

    if (std::find(entries.cbegin(), entries.cend(), rccEntryName) == entries.cend())
    {
        throw utils::IOError("No rcc file found in surface package " + this->path);
    }

    const KArchiveEntry* rccEntry = rootDir->entry(rccEntryName);
    if (rccEntry->isDirectory())
    {
        throw utils::IOError("rcc file is not a file in surface package " + this->path);
    }

    const KArchiveFile* rccFile = dynamic_cast<const KArchiveFile*>(rccEntry);
    this->resourceData = rccFile->data();

    const unsigned char* data = reinterpret_cast<const uchar*>(this->resourceData.data());
    if (!QResource::registerResource(data))
    {
        throw utils::IOError("Failed to register  " + this->path);
    }

    QFile jfile(utils::makePath(
        path::surface, utils::makeFileName(this->objectName(), utils::fileExtensions::surfaceDefinition)));
    if (!jfile.open(QIODevice::ReadOnly))
    {
        throw utils::IOError(
            "Failed to open surface definition from package " + this->path + ": " + jfile.errorString());
    }

    QJsonParseError parseError;
    const QJsonDocument jdoc = QJsonDocument::fromJson(jfile.readAll(), &parseError);

    if (parseError.error != QJsonParseError::NoError)
    {
        throw utils::ValueError("Error parsing surface definition file from surface package " + this->path + ". " +
            parseError.errorString() + " at " + parseError.offset);
    }

    const QJsonObject jobj = jdoc.object();

    this->tileWidth = jobj["tileWidth"].toInt();
    this->tileHeight = jobj["tileHeight"].toInt();
    this->normalGridColor = QColor(jobj["normalGridColor"].toString());
    this->focusGridColor = QColor(jobj["focusGridColor"].toString());

    emit tileWidthChanged();
    emit tileHeightChanged();
    emit tileSizeChanged();
    emit normalGridColorChanged();
    emit focusGridColorChanged();

    if (!this->hexMask.load(path::hexagonMask))
    {
        throw utils::IOError("Hexagon mask not found in surface package " + this->path);
    }

    checkMissingImages(this);

    wInfo << "Succesfully activated surface " << this->objectName();
}

void WorldSurface::deactivate()
{
    const unsigned char* data = reinterpret_cast<const uchar*>(this->resourceData.data());
    if (!QResource::unregisterResource(data))
    {
        throw utils::IOError("Failed to unregister  " + this->path);
    }

    wInfo << "Succesfully deactivated surface " << this->objectName();
}

QSGTexture* WorldSurface::getTexture(const QObject* object, QQuickWindow* window)
{
    QSGTexture* texture{nullptr};

    const auto textureKey = std::make_pair(object, window);
    const auto it = this->objectTextures.find(textureKey);

    if (it == this->objectTextures.end())
    {
        texture = createTexture(QImage(objectPath(object)), window);

        if (texture != nullptr)
        {
            this->objectTextures.emplace(
                textureKey, std::unique_ptr<QSGTexture, utils::DelayedQObjectDeleter>(texture));
            wDebug << "Created texture for " << object;
        }
    }
    else
    {
        texture = it->second.get();
    }

    return texture;
}

QSGTexture* WorldSurface::getTexture(Image image, QQuickWindow* window)
{
    QSGTexture* texture{nullptr};

    const QString path(imagePath(image));
    const auto textureKey = std::make_pair(path, window);
    const auto it = this->staticTextures.find(textureKey);

    if (it == this->staticTextures.end())
    {
        texture = createTexture(QImage(path), window);

        if (texture != nullptr)
        {
            this->staticTextures.emplace(
                textureKey, std::unique_ptr<QSGTexture, utils::DelayedQObjectDeleter>(texture));
            wDebug << "Created texture for " << path;
        }
    }
    else
    {
        texture = it->second.get();
    }

    return texture;
}

QUrl WorldSurface::getObjectImageUrl(const QObject* object) const
{
    const QString fullClassName{object->metaObject()->className()};
    if (visualClasses.find(fullClassName) == visualClasses.end())
    {
        return QUrl();
    }
    else
    {
        const QString className = fullClassName.section("::", -1);
        return QUrl(resourceSchema +
            utils::makePath(path::surface, className, utils::makeFileName(object->objectName(), fileExtension)));
    }
}

QString WorldSurface::getObjectImagePath(const QObject* object) const
{
    const QString fullClassName{object->metaObject()->className()};
    if (visualClasses.find(fullClassName) == visualClasses.end())
    {
        return QString();
    }
    else
    {
        const QString className = fullClassName.section("::", -1);
        return QString(
            utils::makePath(path::surface, className, utils::makeFileName(object->objectName(), fileExtension)));
    }
}

QUrl WorldSurface::getImageUrl(Image image) const
{
    return QUrl(resourceSchema + getImagePath(image));
}

QString WorldSurface::getImagePath(Image image) const
{
    return imagePath(image);
}

void WorldSurface::parseHeader(const QByteArray& header)
{
    QJsonParseError parseError;
    QJsonDocument jdoc = QJsonDocument::fromJson(header, &parseError);

    if (parseError.error != QJsonParseError::NoError)
    {
        throw utils::ValueError("Error parsing surface meta file from surface package " + this->path + ". " +
            parseError.errorString() + " at " + parseError.offset);
    }

    QJsonObject jobj = jdoc.object();

    this->setObjectName(jobj["objectName"].toString());

    this->displayName = jobj["displayName"].toString();
    this->description = jobj["description"].toString();

    emit displayNameChanged();
    emit descriptionChanged();
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

    return true;
}

static QString objectPath(const QObject* const object)
{
    const QString fullClassName{object->metaObject()->className()};
    const QString className = fullClassName.section("::", -1);

    return utils::makePath(path::surface, className, utils::makeFileName(object->objectName(), fileExtension));
}

static QString imagePath(WorldSurface::Image)
{
    return QString();
}

static void checkMissingImages(const WorldSurface* const surface)
{
    const auto isObjectImageMissing = [&surface](
        QObject* object) { return QImage(surface->getObjectImagePath(object)).isNull(); };

    const auto isStaticImageMissing = [&surface](
        WorldSurface::Image image) { return QImage(surface->getImagePath(image)).isNull(); };

    const core::World* world = surface->getWorld();

    const auto& armyTypes = world->getArmyTypes();
    if (std::any_of(armyTypes.cbegin(), armyTypes.cend(), isObjectImageMissing))
    {
        throw utils::IOError("Failed to find one or more army-type images");
    }

    const auto& banner = world->getBanners();
    if (std::any_of(banner.cbegin(), banner.cend(), isObjectImageMissing))
    {
        throw utils::IOError("Failed to find one or more banner images");
    }

    const auto& settlementTypes = world->getSettlementTypes();
    if (std::any_of(settlementTypes.cbegin(), settlementTypes.cend(), isObjectImageMissing))
    {
        throw utils::IOError("Failed to find one or more settlement-type images");
    }

    const auto& terrainTypes = world->getTerrainTypes();
    if (std::any_of(terrainTypes.cbegin(), terrainTypes.cend(), isObjectImageMissing))
    {
        throw utils::IOError("Failed to find one or more terrain-type images");
    }

    const auto& unitTypes = world->getUnitTypes();
    if (std::any_of(unitTypes.cbegin(), unitTypes.cend(), isObjectImageMissing))
    {
        throw utils::IOError("Failed to find one or more unit-type images");
    }

    if (std::any_of(staticImages.cbegin(), staticImages.cend(), isStaticImageMissing))
    {
        throw utils::IOError("Failed to find one or more static images");
    }
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
