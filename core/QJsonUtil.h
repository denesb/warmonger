#ifndef CORE_QJSON_UTIL_H
#define CORE_QJSON_UTIL_H

#include <QObject>
#include <QString>
#include <QSize>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include "core/Exception.h"

namespace warmonger {
namespace core {

/**
 * Get the objectName of a QObject.
 */
inline QString qObjectName(const QObject * const object)
{
    return object->objectName();
}

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

/**
 * Callable reference resolver.
 *
 * This is a helper class which stores the parent and provides a
 * function call operator to resolve references of type T with the
 * stored parent.
 */
template<typename T>
class ReferenceResolver
{
public:
    ReferenceResolver(const QObject * const parent) :
        parent(parent)
    {}

    T * operator()(const QString &objectName) const
    {
        return resolveReference<T>(objectName, this->parent);
    }

    T * operator()(const QJsonValue &value) const
    {
        const QString objectName = value.toString();
        return resolveReference<T>(objectName, this->parent);
    }

private:
    const QObject * const parent;
};

/**
 * QJsonValue to bool.
 */
inline bool qJsonValueToBool(const QJsonValue &value)
{
    return value.toBool();
}

/**
 * QJsonValue to int.
 */
inline int qJsonValueToInt(const QJsonValue &value)
{
    return value.toInt();
}

/**
 * QJsonValue to double.
 */
inline double qJsonValueToDouble(const QJsonValue &value)
{
    return value.toDouble();
}

/**
 * QJsonValue to QString.
 */
inline QString qJsonValueToQString(const QJsonValue &value)
{
    return value.toString();
}

/**
 * Construct a QJsonValue object directly from T.
 *
 * Only usable if QJsonValue has a constructor with T (or a type
 * implicitly convertable to T).
 */
template<typename T>
QJsonValue constructQJsonValue(const T &value)
{
    return QJsonValue(value);
}

/**
 * Construct T from QJsonObject.
 *
 * T must have the same constructor as QObject and a fromJson
 * member accepting a QJsonObject.
 */
template<typename T>
T * objectFromQJsonObject(const QJsonValue &value, QObject *parent)
{
    T *instance = new T(parent);
    instance->fromJson(value.toObject());
    return instance;
}

/**
 * Object factory function object.
 *
 * Create instances of T with the stored parent from QJsonObjects.
 * For T the same restrictions apply as for T at `constructFromQJsonObject`.
 */
template<typename T>
class ObjectConstructor
{
public:
    ObjectConstructor(QObject *parent) :
        parent(parent)
    {
    }

    T * operator()(const QJsonValue &value)
    {
        return objectFromQJsonObject<T>(value, this->parent);
    }

private:
    QObject *parent;
};

/**
 * Convert T to QJsonObject.
 *
 * T must have a toJson method.
 */
template<typename T>
QJsonObject objectToQJsonObject(const T *object)
{
    return object->toJson();
}

/**
 * Convert Container to QJsonArray.
 *
 * The convertFunc function converts Container::value_type to
 * QJsonValue.
 */
template<typename Container, typename ConvertFunc>
QJsonArray toQJsonArray(Container container, ConvertFunc convertFunc)
{
    QJsonArray array;

    std::transform(
        container.cbegin(),
        container.cend(),
        std::back_inserter(array),
        convertFunc
    );

    return array;
}

/**
 * Convert QJsonArray to Container.
 *
 * Container must be an STL-compatible container having a push_back
 * method. The convertFunc function converts QJsonValue to
 * Container::value_type.
 */
template<typename Container, typename ConvertFunc>
Container fromQJsonArray(const QJsonArray &array, ConvertFunc convertFunc)
{
    Container container;

    std::transform(
        array.begin(),
        array.end(),
        std::back_inserter(container),
        convertFunc
    );

    return container;
}

/**
 * Convert a Qt-style mapping-type Container to QJsonObject.
 *
 * Qt-style means that the iterator has a key() and value() member
 * returning the key and value respectively.
 * For the conversion a `convertKey` and a `convertValue` function must
 * be supplied, where `convertKey` converts from Container::key_type to
 * QString and `convertValue converts from Container::mapping_type to
 * QJsonValue.
 */
template<typename Container, typename ConvertKeyFunc, typename ConvertValueFunc>
QJsonObject toQJsonObject(
    Container container,
    ConvertKeyFunc convertKey,
    ConvertValueFunc convertValue
)
{
    QJsonObject object;

    for(auto it = container.cbegin(); it != container.cend(); it++)
    {
        QString key = convertKey(it.key());
        QJsonValue value = convertValue(it.value());

        object[key] = value;
    }

    return object;
}

/**
 * Convert QJsonObject to Container.
 *
 * Container must be a mapping type supporting assigment via the []
 * operator. For the conversion a `convertKey` and a `convertValue`
 * function must be supplied, where `convertKey` converts from QString
 * to Container::key_type and `convertValue converts from QJsonValue to
 * Container::mapped_type.
 */
template<typename Container, typename ConvertKeyFunc, typename ConvertValueFunc>
Container fromQJsonObject(
    const QJsonObject &object,
    ConvertKeyFunc convertKey,
    ConvertValueFunc convertValue
)
{
    Container container;

    for(auto it = object.begin(); it != object.end(); it++)
    {
        typename Container::key_type key = convertKey(it.key());
        typename Container::mapped_type value = convertValue(it.value());

        container[key] = value;
    }

    return container;
}

/**
 * Convert QJsonObject to QSize.
 */
QSize sizeFromJson(const QJsonObject &obj);

/**
 * Convert QSize to QJsonObject.
 */
QJsonObject sizeToJson(const QSize &size);

/**
 * Load a QJsonDocument from path.
 *
 * Throws IOError if the path is not readable, or JsonParseError if the
 * Json document is not parsable.
 */
QJsonDocument loadJsonDocument(const QString &path);

/**
 * Save a QJsonDocument to path.
 *
 * Throws IOError if path is not writable.
 */
void saveJsonDocument(const QString &path, const QJsonDocument &doc);

} // namespace core
} // namespace warmonger

#endif // CORE_QJSON_UTIL_H
