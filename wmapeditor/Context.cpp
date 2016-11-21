#include "core/MapGenerator.h"
#include "io/File.h"
#include "io/JsonSerializer.h"
#include "io/JsonUnserializer.h"
#include "utils/Constants.h"
#include "utils/Exception.h"
#include "utils/Settings.h"
#include "utils/Utils.h"
#include "wmapeditor/Context.h"

namespace warmonger {
namespace wmapeditor {

const QString campaignMapObjectName{"newCampaignMap"};
const QString campaignMapDisplayName{"New campaign map"};
const QString unknownErrorMessage{"Unknown error"};

static QString nextCampaignMapName(const std::vector<core::CampaignMap*>& campaignMaps);
static QString defaultPath(const core::World* world, const core::CampaignMap* campaignMap);

struct WorldInjector
{
    WorldInjector(const std::vector<core::World*>& worlds)
        : worlds(worlds)
    {
    }

    void operator()(const QString& className, const QString& objectName, io::Context& ctx);

    const std::vector<core::World*>& worlds;
};

void Context::create(warmonger::core::World* world)
{
    core::CampaignMap* map = new core::CampaignMap(this);
    map->setObjectName(nextCampaignMapName(this->campaignMaps));
    map->setDisplayName(campaignMapDisplayName);
    map->setWorld(world);

    const std::vector<core::TerrainType*> terrainTypes = world->getTerrainTypes();
    if (!terrainTypes.empty())
    {
        const std::vector<core::MapNode*> nodes = core::generateMapNodes(10);
        core::generateMapNodeNames(nodes);
        core::generateMapNodeTerrainTypes(nodes, terrainTypes);

        map->setMapNodes(nodes);
    }

    const std::vector<core::Civilization*>& civilizations = world->getCivilizations();

    std::random_device rd;
    std::mt19937 mtd(rd());
    std::uniform_int_distribution<std::size_t> civsDist(0, civilizations.size() - 1);

    map->createFaction(civilizations.at(civsDist(mtd)));
    map->createFaction(civilizations.at(civsDist(mtd)));

    this->setCampaignMap(map);
}

bool Context::save()
{
    return this->saveAs(this->lastPath.isEmpty() ? defaultPath(this->world, this->campaignMap) : this->lastPath);
}

bool Context::saveAs(const QString& path)
{
    try
    {
        io::JsonSerializer serializer;
        io::writeCampaignMap(this->campaignMap, path, serializer);
    }
    catch (utils::IOError& e)
    {
        this->lastErrorCategory = ErrorCategory::IOError;
        this->lastErrorMessage = e.getMessage();
        return false;
    }
    catch (utils::Exception& e)
    {
        this->lastErrorCategory = ErrorCategory::InternalError;
        this->lastErrorMessage = e.getMessage();
        return false;
    }
    catch (...)
    {
        this->lastErrorCategory = ErrorCategory::UnknownError;
        this->lastErrorMessage = unknownErrorMessage;
        return false;
    }

    this->lastPath = path;

    return true;
}

bool Context::load(const QString& path)
{
    try
    {
        WorldInjector injector(this->worlds);
        io::Context ctx(std::function<void(const QString&, const QString&, Context&)>(injector));

        io::JsonUnserializer unserializer;
        this->setCampaignMap(io::readCampaignMap(path, unserializer));
    }
    catch (utils::IOError& e)
    {
        this->lastErrorCategory = ErrorCategory::IOError;
        this->lastErrorMessage = e.getMessage();
        return false;
    }
    catch (utils::ValueError& e)
    {
        this->lastErrorCategory = ErrorCategory::ContentError;
        this->lastErrorMessage = e.getMessage();
        return false;
    }
    catch (utils::Exception& e)
    {
        this->lastErrorCategory = ErrorCategory::InternalError;
        this->lastErrorMessage = e.getMessage();
        return false;
    }
    catch (...)
    {
        this->lastErrorCategory = ErrorCategory::UnknownError;
        this->lastErrorMessage = unknownErrorMessage;
        return false;
    }

    this->lastPath = path;

    return true;
}

static QString nextCampaignMapName(const std::vector<core::CampaignMap*>& campaignMaps)
{
    std::vector<QString> names;

    names.reserve(campaignMaps.size());

    for (const core::CampaignMap* campaignMap : campaignMaps)
        names.push_back(campaignMap->objectName());

    std::size_t postFix{0};
    QString objectName{campaignMapObjectName + QString::number(postFix)};

    while (std::find(names.begin(), names.end(), objectName) != names.end())
        objectName = campaignMapObjectName + QString::number(++postFix);

    return objectName;
}

static QString defaultPath(const core::World* world, const core::CampaignMap* campaignMap)
{
    const QString worldsPath{utils::settingsValue(utils::SettingsKey::worldsDir).toString()};
    const QString fileName{utils::makeFileName(campaignMap->objectName(), utils::fileExtensions::mapDefinition)};
    return utils::makePath(worldsPath, world->objectName(), utils::paths::maps, fileName);
}

void WorldInjector::operator()(const QString& className, const QString& objectName, io::Context& ctx)
{
    if (className == core::World::staticMetaObject.className())
    {
        auto it = std::find_if(this->worlds.begin(), this->worlds.end(), utils::QObjectFinder(objectName));
        if (it != this->worlds.end())
            io::addWorldToContext(ctx, *it);
    }
}

} // namespace wmapeditor
} // namespace warmonger
