#include <iostream>
#include <memory>

#include <QString>

#include "io/SanityCheck.h"
#include "tools/Utils.h"
#include "utils/Logging.h"

using namespace warmonger;

/**
 * Sanity-check a world.
 *
 * A "sane" world can be loaded without exceptions and it has at least one of
 * every type.
 */
int main(int argc, char* const argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: wcheck_world /path/to/world.wwd" << std::endl;
        return 1;
    }

    boost::shared_ptr<std::stringstream> logStream = tools::setupLogging();

    QString path{argv[1]};

    wInfo << "path: " << path;

    if (!io::isWorldSane(path))
        FAIL(1);

    return 0;
}
