#ifndef CORE_JSON_UTIL_HPP
#define CORE_JSON_UTIL_HPP

#include <QObject>
#include <QString>
#include <QList>
#include <QMap>
#include <QPoint>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>

#include "core/Exception.h"

namespace warmonger {
namespace core {

template<typename T>
T * newFromJson(const QJsonObject &obj, QObject *parent)
{
    T *instance = new T(parent);
    instance->fromJson(obj);
    return instance;
}

template<typename T>
QList<T *> newListFromJson(const QJsonArray &array, QObject *parent)
{
    QList<T *> list;

    for (const QJsonValue v : array)
    {
        T *instance = new T(parent);
        instance->fromJson(v.toObject());
        list.append(instance);
    }

    return std::move(list);
}

template<typename T>
QJsonArray listToJson(const QList<T *> &list)
{
    QJsonArray array;

    for (const T *instance : list)
    {
        array.append(instance->toJson());
    }

    return std::move(array);
}

template<typename T>
QList<const T *> referenceListFromJson(const QJsonArray &array, QObject *owner)
{
    QList<const T *> list;
    QObject *parent = owner->parent();
    if (parent == nullptr)
    {
        wError("core.JsonUtil") << "Parent of owner " << owner->objectName() << " is null";
        throw Exception(Exception::NullPointer);
    }

    for (const QJsonValue v : array)
    {
        const QString name = v.toString();
        const T * instance = parent->findChild<T *>(name);

        if (instance == nullptr)
        {
            wError("core.JsonUtil") << "Unable to find child " << name << " of " << parent->objectName();
            throw Exception(Exception::UnresolvedReference, {"T", name});
        }

        list.append(instance);
    }

    return std::move(list);
}

template<typename T>
QJsonArray referenceListToJson(const QList<const T *> &list)
{
    QJsonArray array;

    for (const T * instance : list)
    {
        array.append(instance->objectName());
    }

    return std::move(array);
}

template<typename T>
QMap<const T *, int> objectValueMapFromJson(const QJsonObject &obj, const QObject * const owner)
{
    QMap<const T *, int> map;
    QObject *parent = owner->parent();
    if (parent == nullptr)
    {
        wError("core.JsonUtil") << "Parent of owner " << owner->objectName() << " is null";
        throw Exception(Exception::NullPointer);
    }

    QJsonObject::const_iterator it;
    for(it = obj.constBegin(); it != obj.constEnd(); it++)
    {
        const QString name = it.key();
        const T *instance = parent->findChild<T *>(name);

        if (instance == nullptr)
        {
            wError("core.JsonUtil") << "Unable to find child " << name << " of " << parent->objectName();
            throw Exception(Exception::UnresolvedReference, {"T", name});
        }

        map[instance] = it.value().toInt();
    }

    return std::move(map);
}

template<typename T>
QJsonObject objectValueMapToJson(const QMap<const T *, int> &map)
{
    QJsonObject obj;

    typename QMap<const T *, int>::const_iterator it;
    for(it = map.constBegin(); it != map.constEnd(); it++)
    {
        const T *instance = it.key();
        obj[instance->objectName()] = it.value();
    }
    
    return std::move(obj);
}

} // namespace core
} // namespace warmonger

#endif // CORE_JSON_UTIL_HPP
