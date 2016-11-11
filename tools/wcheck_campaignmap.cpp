#include <iostream>
#include <memory>

#include "core/CampaignMap.h"
#include "io/File.h"
#include "io/JsonUnserializer.h"
#include "tools/Utils.h"
#include "wwrapper/SanityCheck.h"

using namespace warmonger;

/**
 * Sanity-check a campaign-map.
 *
 * A "sane" campaign-map can be loaded without exceptions.
 */
int main(int argc, char* const argv[])
{
    if (argc < 3)
    {
        std::cout << "Usage: wcheck_campaignmap /path/to/world.wwd /path/to/campaignmap.wmd" << std::endl;
        return 1;
    }

    boost::shared_ptr<std::stringstream> logStream = wwrapper::setupLogging();

    QString worldPath{argv[1]};
    QString campaignMapPath{argv[2]};

    wInfo << "world path: " << worldPath;
    wInfo << "campaign-map path: " << campaignMapPath;

    if (!wwrapper::isWorldSane(worldPath))
    {
        wError << "World is not sane";
        FAIL(1);
    }

    io::JsonUnserializer unserializer;
    std::unique_ptr<core::World> world(io::readWorld(worldPath, unserializer));

    wInfo << "Successfully loaded world " << world.get();

    if (!wwrapper::isCampaignMapSane(campaignMapPath, world.get()))
        FAIL(1);

    return 0;
}
