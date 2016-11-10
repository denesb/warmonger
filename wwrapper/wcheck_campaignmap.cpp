#include <iostream>
#include <memory>

#include "core/CampaignMap.h"
#include "io/File.h"
#include "io/JsonUnserializer.h"
#include "utils/Exception.h"
#include "wwrapper/Utils.h"

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

    std::unique_ptr<core::World> world;

    try
    {
        io::JsonUnserializer unserializer;
        world.reset(io::readWorld(worldPath, unserializer));
    }
    catch (std::exception& e)
    {
        wError << "Caught exception while trying to read world: " << e.what();
        FAIL(1);
    }

    wInfo << "Successfully loaded world " << world.get();

    std::unique_ptr<core::CampaignMap> campaignMap;

    try
    {
        io::Context ctx;
        ctx.add(world.get());

        io::JsonUnserializer unserializer(ctx);
        campaignMap.reset(io::readCampaignMap(campaignMapPath, unserializer));
    }
    catch (std::exception& e)
    {
        wError << "Caught exception while trying to read campaignMap: " << e.what();
        FAIL(1);
    }

    wInfo << "Successfully loaded campaignMap " << campaignMap.get();

    return 0;
}
