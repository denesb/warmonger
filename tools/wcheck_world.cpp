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
