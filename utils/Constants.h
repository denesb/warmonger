/** \file
 * Conststants.
 *
 * \copyright (C) 2015-2016 Botond Dénes
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

namespace searchPaths {

const QString world{"World"};
const QString campaignMap{"CampaignMap"};
const QString worldSurface{"WorldSurface"};

} // namespace searchPaths

} // namespace utils
} // namespace warmonger

#endif // CORE_CONSTANTS_H
