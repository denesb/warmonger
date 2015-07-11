#ifndef CORE_MAP_POSITION_H
#define CORE_MAP_POSITION_H

#include <QString>

namespace warmonger {
namespace core {

class MapPosition
{
public:
    MapPosition(int x = 0, int y = 0);
    explicit MapPosition(QString str);
    MapPosition(const MapPosition &other);
    ~MapPosition();

    MapPosition & operator=(const MapPosition &other);
    bool operator==(const MapPosition &other) const;
    bool operator<(const MapPosition &other) const;
    bool operator<=(const MapPosition &other) const;
    bool operator>(const MapPosition &other) const;
    bool operator>=(const MapPosition &other) const;

    int getX() const; 
    void setX(int x);

    int getY() const;
    void setY(int y);
    
    QString toStr() const;

private:
    int x;
    int y;
};

}; // namespace core
}; // namespace warmonger

#endif // CORE_MAP_POSITION_H
