#include <QColor>
#include <QDir>
#include <QStringList>

#include "core/WorldSurface.h"
#include "core/QVariantUtil.h"
#include "ui/ApplicationContext.h"

using namespace warmonger;
using namespace warmonger::ui;

static const QString loggerName{"ui.ApplicationContext"};

ApplicationContext::ApplicationContext(QObject *parent) :
    QObject(parent),
    maps(),
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
    /*
    nameFilters << "*.wmd";

    QFlags<QDir::Filter> filters = QDir::Files | QDir::Readable;

    core::EntityManager *em = core::EntityManager::getInstance();

    for (QString path : QDir::searchPaths("Map"))
    {
        QDir mapsDir(path);

        for (QString mapFile : mapsDir.entryList(nameFilters, filters))
        {
            const QString mapPath = mapsDir.absoluteFilePath(mapFile);
            core::CampaignMap *map{nullptr};
            try
            {
                map = em->loadEntityAs<core::CampaignMap>(mapPath);
            }
            catch (core::JsonParseError &e)
            {
                wError(loggerName) << "Caught " << e
                    << " while loading map " << mapPath;
                continue;
            }
            catch (core::ValueError &e)
            {
                wError(loggerName) << "Caught " << e
                    << " while loading map from " << mapPath;
                continue;
            }
            catch (core::UnresolvedReferenceError &e)
            {
                wError(loggerName) << "Caught " << e
                    << " while loading map " << mapPath;
                continue;
            }

            this->maps << map;
        }
    }
    */

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

void ApplicationContext::newGame(warmonger::core::CampaignMap *map)
{
    if (this->game != nullptr)
    {
        delete this->game;
    }

    emit gameChanged();
}

void ApplicationContext::loadGame(QString objectName)
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
