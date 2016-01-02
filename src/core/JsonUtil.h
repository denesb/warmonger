#ifndef CORE_JSON_UTIL_HPP
#define CORE_JSON_UTIL_HPP

#include <QObject>
#include <QString>
#include <QList>
#include <QMap>
#include <QPoint>
#include <QSize>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include "core/Exception.h"

namespace warmonger {
namespace core {

/**
 * Resolve a reference.
 *
 * Resolving a reference is the process of looking up an object by name.
 * References in Json are stored as strings (the objectName of the
 * QObjects). In the C++ in-memory tree hovever references are stored as
 * pointers to the objects. This function provides a way from
 * object-name to object-pointer. The parent is the parent of the
 * sought-after object.
 */
template<typename T>
T * resolveReference(const QString &objectName, const QObject * const parent)
{
    T *obj = parent->findChild<T *>(objectName);
    if (obj == nullptr)
    {
        throw UnresolvedReferenceError(objectName);
    }

    return obj;
}

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

/**
 * Convert a JSON list of strings into resolved references.
 *
 * The references are resolved against `parent`.
 */
template<typename T>
QList<T *> referenceListFromJson(
    const QJsonArray &array,
    const QObject * const parent
)
{
    QList<T *> list;

    for (const QJsonValue v : array)
    {
        const QString name = v.toString();
        T *instance{nullptr};
        if (!name.isEmpty())
        {
            instance = resolveReference<T>(name, parent);
        }

        list.append(instance);
    }

    return std::move(list);
}

template<typename T>
QJsonArray referenceListToJson(const QList<T *> &list)
{
    QJsonArray array;

    for (T * instance : list)
    {
        array.append(instance->objectName());
    }

    return std::move(array);
}

template<typename Map, typename KeyFromJsonFunc, typename ValueFromJsonFunc>
Map mapFromJson(
    const QJsonObject &obj,
    KeyFromJsonFunc keyFromJson,
    ValueFromJsonFunc valueFromJson
)
{
    Map map;

    for(auto it = obj.constBegin(); it != obj.constEnd(); it++)
    {
        typename Map::key_type key = keyFromJson(it.key());
        typename Map::mapped_type value = valueFromJson(it.value());

        map[key] = value;
    }

    return std::move(map);
}

/**
 * Convert a JSON object to a QMap<T*, int>.
 *
 * The keys of the JSON object are assumed to be references. They will
 * be resolved against the `parent` parameter.
 */
template<typename T>
QMap<const T *, int> objectValueMapFromJson(
    const QJsonObject &obj,
    const QObject * const parent
)
{
    QMap<const T *, int> map;

    QJsonObject::const_iterator it;
    for(it = obj.constBegin(); it != obj.constEnd(); it++)
    {
        const T *instance = resolveReference<T>(it.key(), parent);

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

inline QSize sizeFromJson(const QJsonObject &obj)
{
    return QSize(obj["width"].toInt(), obj["height"].toInt());
}

QJsonObject sizeToJson(const QSize &size);

QJsonDocument loadJsonDocument(const QString &path);
void saveJsonDocument(const QString &path, const QJsonDocument &doc);

} // namespace core
} // namespace warmonger

#endif // CORE_JSON_UTIL_HPP
