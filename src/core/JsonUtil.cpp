#include "core/JsonUtil.hpp"

using namespace warmonger::core;

QJsonObject toJson(const QPoint &point)
{
    QJsonObject obj;

    obj["x"] = point.x();
    obj["y"] = point.y();

    return std::move(obj);
}

QPoint fromJson(const QJsonObject &obj)
{
    QPoint point;

    point.setX(obj["x"].toInt());
    point.setY(obj["y"].toInt());

    return std::move(point);
}
