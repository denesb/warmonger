#ifndef CORE_CONSTANTS_H
#define CORE_CONSTANTS_H

#include <QString>

namespace warmonger {
namespace core {

/*
 * file extension constants
 */ 
namespace fileExtensions {

// Warmonger World Definition
const QString WorldDefinition{"wwd"};

// Warmonger Map Definition
const QString MapDefinition{"wmd"};

// Warmonger Surface Definition
const QString SurfaceDefinition{"wsd"};

// Warmonger Surface Package
const QString SurfacePackage{"wsp"};

// Warmonger Surface Metadata
const QString SurfaceMetadata{"wsm"};

// Qt Resource Definition
const QString QResourceDefinition{"qrc"};

// Qt Binary Resource Data
const QString QResourceData{"rcc"};

} // namespace fileExtensions

} // namespace warmonger
} // namespace warmonger

#endif // CORE_CONSTANTS_H
