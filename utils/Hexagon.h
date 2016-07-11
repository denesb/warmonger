#ifndef W_UTILS_HEXAGON_H
#define W_UTILS_HEXAGON_H

#include <array>

#include <QString>

namespace warmonger {
namespace utils {

/*
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

const std::array<Direction, 6> directions{
    Direction::West,
    Direction::NorthWest,
    Direction::NorthEast,
    Direction::East,
    Direction::SouthEast,
    Direction::SouthWest
};

/**
 * Converts Direction enum to string.
 */
QString direction2str(const Direction d);

/**
 * Converts direction name to Direction enum.
 *
 * Will throw a ValueError if str is not a valid direction name.
 */
Direction str2direction(const QString &str);

/**
 * The opposite direction of direction d.
 */
Direction oppositeDirection(const Direction d);

/**
 * Are directions d1 and d2 opposite to each other?
 */
bool areDirectionsOpposite(const Direction d1, const Direction d2);

/**
 * The neighbouring directions of direction d.
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
 */
std::pair<Direction, Direction> connectingDirections(const Direction d1, const Direction d2);

} // namespace utils
} // namespace warmonger

#endif // W_UTILS_HEXAGON_H
