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

#include <QString>

namespace warmonger {

namespace core {
class Map;
class World;
}

namespace io {

/**
 * Write the world to path.
 *
 * Serialize and write the world to the file at path.
 *
 * \param world the world
 * \param path the path where the world will be saved
 *
 * \throw utils::IOError if the file at path is not writeable
 */
void writeWorld(const core::World* const world, const QString& path);

/**
 * Read the world from path.
 *
 * Read and unserialize the world from the file at path.
 * The path can be a relative or absolute path to a world description
 * file or just a name. In the latter case the world will be looked up
 * in the worlds dir as set in the settings.
 *
 * \param path the path to the world file or the name of the world
 *
 * \return the loaded world
 *
 * \throw utils::IOError if the file at path is not readable
 * \throw utils::ValueError if there is a problem with unserializing
 */
std::unique_ptr<core::World> readWorld(const QString& path);

/**
 * Write the map to path.
 *
 * Serialize and write the map to the file at path.
 *
 * \param map the map
 * \param path the path where the map will be saved
 *
 * \throw utils::IOError if the file at path is not writeable
 */
void writeMap(const core::Map* const map, const QString& path);

/**
 * Read the map from path.
 *
 * Read and unserialize the map from the file at path.
 *
 * \param path the path to the map file
 * \param world the world this map belongs to
 *
 * \return the loaded map
 *
 * \throw utils::IOError if the file at path is not readable
 * \throw utils::ValueError if there is a problem with unserializing
 */
std::unique_ptr<core::Map> readMap(const QString& path, core::World* world);

} // namespace warmonger
} // namespace io

#endif // IO_FILE_H
