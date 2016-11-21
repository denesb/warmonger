/** \file
 * Hexagon functions.
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

#ifndef W_UTILS_HEXAGON_H
#define W_UTILS_HEXAGON_H

#include <array>

#include <QString>

namespace warmonger {
namespace core {

/**
 * Hexagon directions.
 *
 *  NW  /\  NE
 *    /    \
 * W |      | E
 *   |      |
 *    \    /
 *  SW  \/  SE
 */
enum class Direction
{
    West,
    NorthWest,
    NorthEast,
    East,
    SouthEast,
    SouthWest
};

const std::array<Direction, 6> directions{Direction::West,
    Direction::NorthWest,
    Direction::NorthEast,
    Direction::East,
    Direction::SouthEast,
    Direction::SouthWest};

/**
 * Converts Direction enum to string.
 *
 * \param d the direction
 *
 * \return the string representation
 */
QString direction2str(const Direction d);

/**
 * Converts direction name to Direction enum.
 *
 * \param str the string representation of the enum
 *
 * \return the direction
 *
 * \throw utils::ValueError if str is not a valid direction name
 */
Direction str2direction(const QString& str);

/**
 * The opposite direction of direction d.
 *
 * \param d the direction
 *
 * \return the direction
 */
Direction oppositeDirection(const Direction d);

/**
 * Are directions d1 and d2 opposite to each other?
 *
 * \param d1 the first direction
 * \param d2 the second direction
 *
 * \return are the directions opposite?
 */
bool areDirectionsOpposite(const Direction d1, const Direction d2);

/**
 * The neighbouring directions of direction d.
 *
 * \param d the direction
 *
 * \return the connection directions
 */
std::pair<Direction, Direction> neighbourDirections(const Direction d);

/**
 * Returns the connecting direction pair for two neighbour directions.
 *
 * Hexagon `a' is neighbour to two other hexagons, `b' and `c' so that `b' and
 * `c' are also neighbours with each other. If `a' is neighbour to `b'
 * along direction `d1' and is neighbour to `d2' along direction `d2' then
 * direction `dc1' is the direction along which `b' is neighbour to `c' and
 * direction `dc2' is the direction alogn which `c' is neighbour ro `b'.
 * The connecting directions is std::pair<dc1, dc2>.
 * See ACSII art below:
 *              ^
 *             / d2
 *       \  /\/
 *        /  / \
 *       | \/  b|
 *       | /\   |
 *      /\/  \ /\
 *    /  / \ /\   \
 *   |  /___|__\___|___ d1 >
 *   |      |   \  |
 *    \  a / \c  \/
 *      \/     \/ \
 *                 \ connecting directions for d1 and d2
 *
 * \param d1 the first direction
 * \param d2 the second direction
 *
 * \return the connection directions
 */
std::pair<Direction, Direction> connectingDirections(const Direction d1, const Direction d2);

} // namespace core
} // namespace warmonger

#endif // W_UTILS_HEXAGON_H
