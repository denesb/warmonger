#ifndef CORE_QVARIANT_UTIL_H
#define CORE_QVARIANT_UTIL_H

#include <algorithm>
#include <iterator>

#include <QString>
#include <QVariant>

#include "core/Exception.h"

namespace warmonger {
namespace core {

/**
 * Convert a Container to a QVariantList.
 *
 * Container::value_type must be a convertable to QVariant!
 */
template<typename Container>
QVariantList toQVariantList(Container container)
{
    QVariantList list;

    std::transform(
        container.cbegin(),
        container.cend(),
        std::back_inserter(list),
        QVariant::fromValue<typename Container::value_type>
    );

    return list;
}

/**
 * Convert Container to QVariant.
 *
 * Container::value_type must be a convertable to QVariant!
 */
template<typename Container>
QVariant containerToQVariant(Container container)
{
    return QVariant::fromValue<QVariantList>(toQVariantList(container));
}

/**
 * Convert QVariant to T.
 *
 * If the enclosed value has a different type, QVariantTypeError will
 * be thrown.
 */
template<typename T>
T fromQVariant(const QVariant &v)
{
    if (v.canConvert<T>())
        throw QVariantTypeError();

    return v.value<T>();
}

/**
 * Convert a QVariantList to Container.
 *
 * Container must be an STL-compatible container having a push_back
 * method. The elements in the QVariantList must be convartable to
 * Container::value_type, otherwise QVariantTypeError will be thrown!
 */
template<typename Container>
Container fromQVariantList(QVariantList list)
{
    Container container;

    std::transform(
        list.cbegin(),
        list.cend(),
        std::back_inserter(container),
        fromQVariant<typename Container::value_type>
    );

    return container;
}

/**
 * Return the parameter as-is.
 *
 * This function is useful as a convert function, when no conversion
 * is necessary.
 */
template<typename T>
T verbatim(const T &v)
{
    return v;
}

/**
 * Convert a Qt-style mapping-type Container to QVariantMap.
 *
 * Qt-style means that the iterator has a key() and value() member
 * returning the key and value respectively.
 * For the conversion a `convertKey` and a `convertValue` function must
 * be supplied, where `convertKey` converts from Container::key_type to
 * QString and `convertValue converts from Container::mapping_type to
 * QVariant.
 */
template<typename Container, typename ConvertKeyFunc, typename ConvertValueFunc>
QVariantMap toQVariantMap(
    Container container,
    ConvertKeyFunc convertKey,
    ConvertValueFunc convertValue
)
{
    QVariantMap map;

    for (auto it = container.cbegin(); it != container.cend(); it++)
    {
        QString key = convertKey(it.key());
        QVariant value = convertValue(it.value());
        map[key] = value;
    }

    return map;
}

/**
 * Convert QVariantMap to Container.
 *
 * Container must be a mapping type supporting assigment via the []
 * operator. For the conversion a `convertKey` and a `convertValue`
 * function must be supplied, where `convertKey` converts from QString
 * to Container::key_type and `convertValue converts from QVariant to
 * Container::mapped_type.
 */
template<typename Container, typename ConvertKeyFunc, typename ConvertValueFunc>
Container fromQVariantMap(
    QVariantMap map,
    ConvertKeyFunc convertKey,
    ConvertValueFunc convertValue
)
{
    Container container;

    for (auto it = map.cbegin(); it != map.cend(); it++)
    {
        typename Container::key_type key = convertKey(it.key());
        typename Container::mapped_type value = convertValue(it.value());
        container[key] = value;
    }

    return container;
}

} // namespace core
} // namespace warmonger

#endif // CORE_QVARIANT_UTIL_H