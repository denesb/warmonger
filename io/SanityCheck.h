/** \file
 * Sanity-check functions.
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

#ifndef W_WWRAPPER_SANITY_CHECK_H
#define W_WWRAPPER_SANITY_CHECK_H

class QString;

namespace warmonger {

namespace core {
class World;
} // namespace core

namespace io {

/**
 * Is the world sane?
 *
 * A "sane" world can be loaded without exceptions and it has at least one of
 * every type.
 *
 * \param world the world to check
 *
 * \return is the world sane
 */
bool isWorldSane(const core::World& world);

/**
 * Is the world sane?
 *
 * \see isWorldSane(core::World*)
 *
 * \param path the path to the world
 *
 * \return is the world sane
 */
bool isWorldSane(const QString& path);

/**
 * Is the campaign-map sane?
 *
 * A "sane" campaign-map can be loaded without exceptions.
 *
 * \param path the path to the campaign-map
 * \param world a *sane* world
 *
 * \return is the campaign-map sane
 *
 * \see isWorldSane
 */
bool isCampaignMapSane(const QString& path, core::World* world);

} // namespace io
} // namespace warmonger

#endif // W_WWRAPPER_SANITY_CHECK_H
