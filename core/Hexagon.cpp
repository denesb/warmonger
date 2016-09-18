#include <map>

#include "core/Hexagon.h"
#include "utils/Exception.h"

namespace warmonger {
namespace core {

namespace {

std::pair<Direction, Direction> connectingDirectionsPrivate(const Direction d1, const Direction d2);
}

QString direction2str(const Direction d)
{
    QString dStr;

    switch (d)
    {
        case Direction::West:
            dStr = "West";
            break;

        case Direction::NorthWest:
            dStr = "NorthWest";
            break;

        case Direction::NorthEast:
            dStr = "NorthEast";
            break;

        case Direction::East:
            dStr = "East";
            break;

        case Direction::SouthEast:
            dStr = "SouthEast";
            break;

        case Direction::SouthWest:
            dStr = "SouthWest";
            break;
    }

    return dStr;
}

Direction str2direction(const QString& str)
{
    static const std::map<QString, Direction> str2dir{{"West", Direction::West},
        {"NorthWest", Direction::NorthWest},
        {"NorthEast", Direction::NorthEast},
        {"East", Direction::East},
        {"SouthEast", Direction::SouthEast},
        {"SouthWest", Direction::SouthWest}};

    if (str2dir.find(str) == str2dir.end())
    {
        throw utils::ValueError(str + " is not a valid direction");
    }

    return str2dir.at(str);
}

Direction oppositeDirection(const Direction d)
{
    static const std::map<Direction, Direction> oppositeDirs{{Direction::West, Direction::East},
        {Direction::NorthWest, Direction::SouthEast},
        {Direction::NorthEast, Direction::SouthWest},
        {Direction::East, Direction::West},
        {Direction::SouthEast, Direction::NorthWest},
        {Direction::SouthWest, Direction::NorthEast}};

    return oppositeDirs.at(d);
}

bool areDirectionsOpposite(const Direction d1, const Direction d2)
{
    return oppositeDirection(d1) == d2;
}

std::pair<Direction, Direction> neighbourDirections(const Direction d)
{
    std::pair<Direction, Direction> nd;
    switch (d)
    {
        case Direction::West:
            nd = std::make_pair(Direction::SouthWest, Direction::NorthWest);
            break;

        case Direction::NorthWest:
            nd = std::make_pair(Direction::West, Direction::NorthEast);
            break;

        case Direction::NorthEast:
            nd = std::make_pair(Direction::NorthWest, Direction::East);
            break;

        case Direction::East:
            nd = std::make_pair(Direction::NorthEast, Direction::SouthEast);
            break;

        case Direction::SouthEast:
            nd = std::make_pair(Direction::East, Direction::SouthWest);
            break;

        case Direction::SouthWest:
            nd = std::make_pair(Direction::SouthEast, Direction::West);
            break;
    }

    return nd;
}

std::pair<Direction, Direction> connectingDirections(const Direction d1, const Direction d2)
{
    try
    {
        return connectingDirectionsPrivate(d1, d2);
    }
    catch (utils::ValueError& e)
    {
    }

    try
    {
        std::pair<Direction, Direction> inverse = connectingDirectionsPrivate(d2, d1);
        return std::make_pair(std::get<1>(inverse), std::get<0>(inverse));
    }
    catch (utils::ValueError& e)
    {
    }

    throw utils::ValueError(
        "Cannot find connecting directions for direction `" + direction2str(d1) + "' and `" + direction2str(d2));
}

namespace {

std::pair<Direction, Direction> connectingDirectionsPrivate(const Direction d1, const Direction d2)
{
    if (d1 == Direction::West && d2 == Direction::NorthWest)
    {
        return std::make_pair(Direction::NorthEast, Direction::SouthWest);
    }
    else if (d1 == Direction::NorthWest && d2 == Direction::NorthEast)
    {
        return std::make_pair(Direction::East, Direction::West);
    }
    else if (d1 == Direction::NorthEast && d2 == Direction::East)
    {
        return std::make_pair(Direction::SouthEast, Direction::NorthWest);
    }
    else if (d1 == Direction::East && d2 == Direction::SouthEast)
    {
        return std::make_pair(Direction::SouthWest, Direction::NorthEast);
    }
    else if (d1 == Direction::SouthEast && d2 == Direction::SouthWest)
    {
        return std::make_pair(Direction::West, Direction::East);
    }
    else if (d1 == Direction::SouthWest && d2 == Direction::West)
    {
        return std::make_pair(Direction::NorthWest, Direction::SouthEast);
    }
    else
    {
        throw utils::ValueError(
            "Cannot find connecting directions for direction `" + direction2str(d1) + "' and `" + direction2str(d2));
    }
}
}

} // namespace core
} // namespace warmonger
