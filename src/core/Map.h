#include <QJsonObject>
#include <QList>

#ifndef CORE_MAP_H
#define CORE_MAP_H

namespace core {

class Map :
    public QObject
{
    Q_OBJECT

public:
    Map(const QJsonObject &json, QObject *parent = nullptr);

private:
};

}; // namespace core

#endif // CORE_MAP_H
