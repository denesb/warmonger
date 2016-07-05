#include <QColor>
#include <QDir>
#include <QStringList>

#include "core/QVariantUtil.h"
#include "io/File.h"
#include "io/JsonUnserializer.h"
#include "ui/ApplicationContext.h"
#include "ui/WorldSurface.h"
#include "Constants.h"

using namespace warmonger;
using namespace warmonger::ui;

static const QString loggerName{"ui.ApplicationContext"};

ApplicationContext::ApplicationContext(QObject *parent) :
    QObject(parent),
    game(nullptr)
{
}

ApplicationContext::~ApplicationContext()
{
}

QVariantList ApplicationContext::readMaps() const
{
    return core::toQVariantList(this->maps);
}

core::Game * ApplicationContext::getGame() const
{
    return this->game;
}

void ApplicationContext::loadMaps()
{
    QStringList nameFilters;
    nameFilters << "*." + fileExtensions::mapDefinition;

    const QFlags<QDir::Filter> filters = QDir::Files | QDir::Readable;

    io::JsonUnserializer worldUnserializer;

    for (QString worldPath : QDir::searchPaths("Worlds"))
    {
        core::World *world{nullptr};

        try
        {
            world = io::readWorld(worldPath, &worldUnserializer);
        }
        catch(const Exception &error)
        {
            wError(loggerName) << "Error loading world " << worldPath << ", " << error.getMessage();
            continue;
        }

        world->setParent(this);

        io::Context worldContext;
        worldContext.add(world);

        QDir mapsDir(worldPath + "/" + paths::maps);
        if (!mapsDir.exists())
        {
            wInfo(loggerName) << "World " << world->objectName() << " does not have a maps directory";
            continue;
        }

        io::JsonUnserializer mapUnserializer(worldContext);

        const QStringList mapEntries = mapsDir.entryList(nameFilters, filters);
        for (const QString &mapFile : mapEntries)
        {
            const QString mapPath = mapsDir.absoluteFilePath(mapFile);

            core::CampaignMap *map{nullptr};
            try
            {
                map = io::readCampaignMap(mapPath, &mapUnserializer);
            }
            catch (const Exception &error)
            {
                wError(loggerName) << "Error loading map " << mapPath << ", " << error.getMessage();
                continue;
            }

            map->setParent(this);

            this->maps << map;
        }
    }

    emit mapsChanged();
}

void ApplicationContext::closeMaps()
{
    for (core::CampaignMap *map : this->maps)
    {
        delete map;
    }
    this->maps.clear();

    emit mapsChanged();
}

void ApplicationContext::newGame(warmonger::core::CampaignMap *)
{
    if (this->game != nullptr)
    {
        delete this->game;
    }

    emit gameChanged();
}

void ApplicationContext::loadGame(QString)
{
    if (this->game != nullptr)
    {
        delete this->game;
    }

    emit gameChanged();
}

void ApplicationContext::closeGame()
{
    if (this->game == nullptr) return;

    delete this->game;
    this->game = nullptr;

    emit gameChanged();
}
