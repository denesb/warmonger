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

static QString objectPath(const QObject* const object);
static void checkMissingImages(const core::World* const world);
static bool isImageMissing(const QString& key);
static bool isOptionalImageMissing(const QObject* const object);
static bool isRequiredImageMissing(const QString& p);
static QSGTexture* createTexture(const QImage& image, QQuickWindow* window);

const std::vector<QString> requiredImagePaths{
    utils::resourcePaths::notFound, utils::resourcePaths::mapEditor::hoverValid};

const std::set<QString> visualClasses{"warmonger::core::TerrainType",
    "warmonger::core::SettlementType",
    "warmonger::core::UnitType",
    "warmonger::core::Banner"};

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

core::World* WorldSurface::getWorld() const
{
    return this->world;
}

QString WorldSurface::getPrefix() const
{
    static const QString prefix = utils::resourcePaths::surface + "/";
    return prefix;
}

QString WorldSurface::getDisplayName() const
{
    return this->displayName;
}

QString WorldSurface::getDescription() const
{
    return this->description;
}

int WorldSurface::getTileWidth() const
{
    return this->tileWidth;
}

int WorldSurface::getTileHeight() const
{
    return this->tileHeight;
}

QSize WorldSurface::getTileSize() const
{
    return QSize(this->tileWidth, this->tileHeight);
}

QColor WorldSurface::getNormalGridColor() const
{
    return this->normalGridColor;
}

QColor WorldSurface::getFocusGridColor() const
{
    return this->focusGridColor;
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
        throw utils::IOError("Failed to open surface package " + this->path + ". " + package.device()->errorString());
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

    QFile jfile(this->getPrefix() + this->objectName() + "." + utils::fileExtensions::surfaceDefinition);
    if (!jfile.open(QIODevice::ReadOnly))
    {
        throw utils::IOError(
            "Failed to open surface definition from package " + this->path + ". " + jfile.errorString());
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

    if (!this->hexMask.load(utils::resourcePaths::hexagonMask))
    {
        throw utils::IOError("Hexagon mask not found in surface package " + this->path);
    }

    checkMissingImages(this->world);

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

QSGTexture* WorldSurface::getTexture(const QString& path, QQuickWindow* window)
{
    QSGTexture* texture{nullptr};

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

QUrl WorldSurface::getImageUrl(QObject* object) const
{
    const QString fullClassName{object->metaObject()->className()};
    if (visualClasses.find(fullClassName) == visualClasses.end())
    {
        return QUrl();
    }
    else
    {
        const QString className = fullClassName.section("::", -1);
        return QUrl(utils::resourcePaths::resourceSchema +
            utils::makePath(utils::resourcePaths::surface,
                        className,
                        utils::makeFileName(object->objectName(), utils::resourcePaths::fileExtension)));
    }
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

static QString objectPath(const QObject* const object)
{
    const QString fullClassName{object->metaObject()->className()};
    const QString className = fullClassName.section("::", -1);

    return utils::makePath(utils::resourcePaths::surface,
        className,
        utils::makeFileName(object->objectName(), utils::resourcePaths::fileExtension));
}

static void checkMissingImages(const core::World* const world)
{
    const auto terrainTypes = world->getTerrainTypes();
    std::for_each(terrainTypes.cbegin(), terrainTypes.cend(), isOptionalImageMissing);

    const auto settlementTypes = world->getSettlementTypes();
    std::for_each(settlementTypes.cbegin(), settlementTypes.cend(), isOptionalImageMissing);

    const auto unitTypes = world->getUnitTypes();
    std::for_each(unitTypes.cbegin(), unitTypes.cend(), isOptionalImageMissing);

    if (std::any_of(requiredImagePaths.cbegin(), requiredImagePaths.cend(), isRequiredImageMissing))
    {
        throw utils::IOError("Failed to find one or more required image resources");
    }
}

static bool isImageMissing(const QString& imagePath)
{
    return QImage(imagePath).isNull();
}

static bool isOptionalImageMissing(const QObject* const object)
{
    const QString path(objectPath(object));
    if (isImageMissing(path))
    {
        wWarning << "Image " << path << " for " << object << " is missing";
        return true;
    }
    else
    {
        return false;
    }
}

static bool isRequiredImageMissing(const QString& path)
{
    if (isImageMissing(path))
    {
        wError << "Image " << path << " is missing";
        return true;
    }
    else
    {
        return false;
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
