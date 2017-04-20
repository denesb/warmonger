/** \file
 * Generic utility functions and function objects.
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

#ifndef W_CORE_UTIL_H
#define W_CORE_UTIL_H

#include <QColor>
#include <QObject>
#include <QString>

namespace warmonger {
namespace core {

class Faction;
class Banner;

/**
 * Create an object-name for the object.
 *
 * The object-name will look like:
 *      className:id
 * Where className is the name of the object's class (namespaces stripped) and
 * the id is the passed in id.
 *
 * \param object the object to generate name for
 * \param id the id
 *
 * \returns the created name
 */
QString createObjectName(const QObject* const object, int id);

/**
 * Split the object-name into class-name and id.
 *
 * \param objectName the object-name to split
 *
 * \returns the object-name components (class-name and id)
 *
 * \see createObjectName
 */
std::pair<QString, int> splitObjectName(const QString& objectName);

/**
 * Split the object-name into class-name and id.
 *
 * \param obj the object whose objectName to split
 *
 * \returns the object-name components (class-name and id)
 *
 * \see createObjectName
 */
inline std::pair<QString, int> splitObjectName(const QObject* const obj)
{
    return splitObjectName(obj->objectName());
}

/**
 * Given the existing factions generate a possible combination.
 *
 * \param factions the factions, used to determine what combinations all taken
 * \param banners the available banners
 * \param colors the available colors
 *
 * \return the next possible combination
 */
std::tuple<Banner*, QColor, QColor> nextAvailableCombination(
    const std::vector<Faction*>& factions, const std::vector<Banner*>& banners, const std::vector<QColor>& colors);

} // namespace core
} // namespace warmonger

#endif // W_CORE_UTIL_HPP
