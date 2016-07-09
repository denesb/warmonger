#ifndef CORE_CONSTANTS_H
#define CORE_CONSTANTS_H

#include <QString>

namespace warmonger {

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

const QString surface{":/surface"};
const QString terrainTypes{surface + "/terrainTypes"};
const QString settlementTypes{surface + "/settlementTypes"};
const QString unitTypes{surface + "/unitTypes"};

const QString fileExtension{"png"};

} // namespace resourcePaths

namespace searchPaths {

const QString world{"World"};
const QString campaignMap{"CampaignMap"};
const QString worldSurface{"WorldSurface"};

} // namespace searchPaths

} // namespace warmonger

#endif // CORE_CONSTANTS_H
