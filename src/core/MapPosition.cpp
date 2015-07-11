#include <QTextStream>

#include "core/MapPosition.h"

using namespace warmonger::core;

static const QString strTemplate("%1,%2");

MapPosition::MapPosition(int x, int y) :
    x(x),
    y(y)
{
}

MapPosition::MapPosition(QString str)
{
    QTextStream s(&str);
    char comma;

    s >> this->x >> comma >> this->y;
}

MapPosition::MapPosition(const MapPosition &other)
{
    this->x = other.x;
    this->y = other.y;
}

MapPosition::~MapPosition()
{
}

MapPosition & MapPosition::operator=(const MapPosition &other)
{
    this->x = other.x;
    this->y = other.y;

    return *this;
}

bool MapPosition::operator==(const MapPosition &other) const
{
    return (this->x == other.x) && (this->y == other.y);
}

bool MapPosition::operator<(const MapPosition &other) const
{
    return (this->x < other.x) && (this->y < other.y);
}

bool MapPosition::operator<=(const MapPosition &other) const
{
    return (this->x <= other.x) && (this->y <= other.y);
}

bool MapPosition::operator>(const MapPosition &other) const
{
    return (this->x > other.x) && (this->y > other.y);
}

bool MapPosition::operator>=(const MapPosition &other) const
{
    return (this->x >= other.x) && (this->y >= other.y);
}

int MapPosition::getX() const
{
    return this->x;
}

void MapPosition::setX(int x)
{
    this->x = x;
}

int MapPosition::getY() const
{
    return this->y;
}

void MapPosition::setY(int y)
{
    this->y = y;
}

QString MapPosition::toStr() const
{
    return std::move(strTemplate.arg(this->x).arg(this->y));
}
