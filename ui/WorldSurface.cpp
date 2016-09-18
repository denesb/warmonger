#include <cmath>
#include <set>

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
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

static QString key(const QObject* const object);
static QString objectPath(const QObject* const object);
static void checkMissingImages(const core::World* const world);
static bool isImageMissing(const QString& key);
static bool isOptionalImageMissing(const QObject* const object);
static bool isRequiredImageMissing(const QString& p);

const std::vector<QString> requiredImagePaths{utils::resourcePaths::notFound,
    utils::resourcePaths::mapEditor::hoverValid};

const std::set<QString> visualClasses{"warmonger::core::TerrainType",
    "warmonger::core::SettlementType",
    "warmonger::core::UnitType"};

WorldSurface::WorldSurface(const QString& path, core::World* world, QQuickWindow* window, QObject* parent)
    : QObject(parent)
    , path(path)
    , world(world)
    , window(window)
    , isTextureSyncOn(window->isSceneGraphInitialized())
    , isTextureSyncPending(false)
{
    QObject::connect(window, &QQuickWindow::sceneGraphInitialized, this, &WorldSurface::turnTextureSyncOn);
    QObject::connect(window, &QQuickWindow::sceneGraphInvalidated, this, &WorldSurface::turnTextureSyncOff);

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

    if (this->isTextureSyncOn)
        this->uploadTextures();
    else
        this->isTextureSyncPending = true;
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

QSGTexture* WorldSurface::getTexture(const QObject* object) const
{
    return this->getTexture(key(object));
}

QSGTexture* WorldSurface::getTexture(const QString& key) const
{
    const auto it = this->textures.find(key);
    if (it == this->textures.end())
    {
        return nullptr;
    }
    else
    {
        return it->second.get();
    }
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

void WorldSurface::uploadTextures()
{
    this->textures.clear();

    void (WorldSurface::*memFn)(const QObject* const) = &WorldSurface::uploadTexture;
    const auto uploadTextureFunc = std::bind(memFn, this, std::placeholders::_1);

    const auto terrainTypes = this->world->getTerrainTypes();
    std::for_each(terrainTypes.cbegin(), terrainTypes.cend(), uploadTextureFunc);

    const auto settlementTypes = this->world->getSettlementTypes();
    std::for_each(settlementTypes.cbegin(), settlementTypes.cend(), uploadTextureFunc);

    const auto unitTypes = this->world->getUnitTypes();
    std::for_each(unitTypes.cbegin(), unitTypes.cend(), uploadTextureFunc);

    std::for_each(requiredImagePaths.cbegin(), requiredImagePaths.cend(), [&](const QString& p) {
        this->uploadTexture(p, QImage(p));
    });

    wInfo << "Textures for surface " << this << " uploaded to GPU";
}

void WorldSurface::uploadTexture(const QObject* const object)
{
    this->uploadTexture(key(object), QImage(objectPath(object)));
}

void WorldSurface::uploadTexture(const QString& textureKey, const QImage& image)
{
    if (image.isNull())
    {
        return;
    }

    this->textures[textureKey] = std::unique_ptr<QSGTexture>(this->window->createTextureFromImage(image));

    wInfo << "Successfully uploaded texture " << textureKey;
}

static QString key(const QObject* const object)
{
    return QString(object->metaObject()->className()) + "/" + object->objectName();
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

} // namespace ui
} // namespace warmonger
