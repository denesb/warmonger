#ifndef CORE_QVARIANT_UTIL_H
#define CORE_QVARIANT_UTIL_H

#include <algorithm>
#include <iterator>

#include <QString>
#include <QVariant>

#include "core/GameEntity.h"
#include "core/Exception.h"

namespace warmonger {
namespace core {

/**
 * Convert a Container to a QVariantList.
 *
 * Container::value_type must be a convertable to QVariant!
 */
template<typename Container>
QVariantList toQVariantList(Container c)
{
    QVariantList vlist;

    std::transform(
        c.cbegin(),
        c.cend(),
        std::back_inserter(vlist),
        QVariant::fromValue<typename Container::value_type>
    );

    return std::move(vlist);
}

/**
 * Convert Container to QVariant.
 *
 * Container::value_type must be a convertable to QVariant!
 */
template<typename Container>
QVariant containerToQVariant(Container c)
{
    return QVariant::fromValue<QVariantList>(toQVariantList(c));
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
Container fromQVariantList(QVariantList vlist)
{
    Container c;

    std::transform(
        vlist.cbegin(),
        vlist.cend(),
        std::back_inserter(c),
        fromQVariant<typename Container::value_type>
    );

    return std::move(c);
}

/**
 * Return the parameter.
 *
 * This function is useful as a convert function, when no conversion
 * is necessary.
 */
template<typename T>
T passThrough(const T &v)
{
    return v;
}

/**
 * Convert a Qt-style mapping-type c to QVariantMap.
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
    Container c,
    ConvertKeyFunc convertKey,
    ConvertValueFunc convertValue
)
{
    QVariantMap vmap;

    for (auto it = c.cbegin(); it != c.cend(); it++)
    {
        QString key = convertKey(it.key());
        QVariant value = convertValue(it.value());
        vmap[key] = value;
    }

    return std::move(vmap);
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
    QVariantMap vmap,
    ConvertKeyFunc convertKey,
    ConvertValueFunc convertValue
)
{
    Container c;

    for (auto it = vmap.cbegin(); it != vmap.cend(); it++)
    {
        typename Container::key_type key = convertKey(it.key());
        typename Container::mapped_type value = convertValue(it.value());
        c[key] = value;
    }

    return std::move(c);
}

} // namespace core
} // namespace warmonger

#endif // CORE_QVARIANT_UTIL_H
