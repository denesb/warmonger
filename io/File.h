/** \file
 * File-level convenience functions.
 *
 * \copyright (C) 2015-2017 Botond Dénes
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

#ifndef IO_FILE_H
#define IO_FILE_H

#include <memory>

#include "io/Serializer.h"
#include "io/Unserializer.h"

namespace warmonger {
namespace io {

/**
 * Write the world to path.
 *
 * Serialize and write the world to the file at path.
 *
 * \param world the world
 * \param path the path where the world will be saved
 * \param serializer the serializer used for serializing the world
 *
 * \throw utils::IOError if the file at path is not writeable
 */
void writeWorld(const core::World* world, const QString& path, io::Serializer& serializer);

/**
 * Read the world from path.
 *
 * Read and unserialize the world from the file at path.
 *
 * \param path the path to the world file
 * \param unserializer the unserializer used for unserializing the world
 *
 * \return the loaded world
 *
 * \throw utils::IOError if the file at path is not readable
 * \throw utils::ValueError if there is a problem with unserializing
 */
std::unique_ptr<core::World> readWorld(const QString& path, io::Unserializer& unserializer);

/**
 * Write the campaign-map to path.
 *
 * Serialize and write the campaign-map to the file at path.
 *
 * \param world the campaign-map
 * \param path the path where the campaign-map will be saved
 * \param serializer the serializer used for serializing the campaign-map
 *
 * \throw utils::IOError if the file at path is not writeable
 */
void writeCampaignMap(const core::CampaignMap* campaignMap, const QString& path, io::Serializer& serializer);

/**
 * Read the campaign-map from path.
 *
 * Read and unserialize the campaign-map from the file at path.
 *
 * \param path the path to the campaign-map file
 * \param unserializer the unserializer used for unserializing the campaign-map
 *
 * \return the loaded campaign-map
 *
 * \throw utils::IOError if the file at path is not readable
 * \throw utils::ValueError if there is a problem with unserializing
 */
std::unique_ptr<core::CampaignMap> readCampaignMap(const QString& path, io::Unserializer& unserializer);

} // namespace warmonger
} // namespace io

#endif // IO_FILE_H
