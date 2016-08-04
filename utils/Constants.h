#ifndef CORE_CONSTANTS_H
#define CORE_CONSTANTS_H

#include <QString>

#include "utils/Utils.h"

namespace warmonger {
namespace utils {

const QString applicationName{"Warmonger"};
const QString organizationName{"Warmonger"};
const QString organizationDomain{"warmonger.org"};

/*
 * file extension constants
 */
namespace fileExtensions {

// Warmonger World Definition
const QString worldDefinition{"wwd"};

// Warmonger Map Definition
const QString mapDefinition{"wmd"};

// Warmonger Surface Definition
const QString surfaceDefinition{"wsd"};

// Warmonger Surface Package
const QString surfacePackage{"wsp"};

// Warmonger Surface Metadata
const QString surfaceMetadata{"wsm"};

// Qt Resource Definition
const QString qResourceDefinition{"qrc"};

// Qt Binary Resource Data
const QString qResourceData{"rcc"};

} // namespace fileExtensions

namespace paths {

// Relative path of the maps directory in the world directory
const QString maps{"maps"};

// Relative path of the surfaces directory in the world directory
const QString surfaces{"surfaces"};

} // namespace paths

namespace resourcePaths {

const QString resourceSchema{"qrc"};
const QString fileExtension{"png"};
const QString prefix{":"};

const QString surface{makePath(prefix, "surface")};

const QString hexagonMask{makePath(surface, QString("hexagonMask.xpm"))};
const QString notFound{makePath(surface, QString("notFound.png"))};

namespace mapEditor {

const QString mapEditor{makePath(surface, "MapEditor")};

const QString hoverValid{makePath(mapEditor, QString("hoverValid.png"))};

} // namespace mapEditor

} // namespace resourcePaths

namespace searchPaths {

const QString world{"World"};
const QString campaignMap{"CampaignMap"};
const QString worldSurface{"WorldSurface"};

} // namespace searchPaths

} // namespace utils
} // namespace warmonger

#endif // CORE_CONSTANTS_H
