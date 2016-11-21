/** \file
 * QVariant utility functions.
 *
 * \copyright (C) 2015-2016 Botond Dénes
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef CORE_QVARIANT_UTIL_H
#define CORE_QVARIANT_UTIL_H

#include <algorithm>
#include <iterator>

#include <QString>
#include <QVariant>

#include "utils/Exception.h"

namespace warmonger {
namespace utils {

inline QString qObjectName(const QObject* const object)
{
    return object->objectName();
}

/**
 * Convert a Container to a QVariantList.
 *
 * Container::value_type must be a convertable to QVariant!
 */
template <typename Container>
QVariantList toQVariantList(Container container)
{
    QVariantList list;

    std::transform(container.cbegin(),
        container.cend(),
        std::back_inserter(list),
        QVariant::fromValue<typename Container::value_type>);

    return list;
}

/**
 * Convert Container to QVariant.
 *
 * Container::value_type must be a convertable to QVariant!
 */
template <typename Container>
QVariant containerToQVariant(Container container)
{
    return QVariant::fromValue<QVariantList>(toQVariantList(container));
}

/**
 * Convert QVariant to T.
 *
 * If the enclosed value has a different type, ValueError will
 * be thrown.
 */
template <typename T>
T fromQVariant(const QVariant& v)
{
    if (v.canConvert<T>())
        throw ValueError();

    return v.value<T>();
}

/**
 * Convert a QVariantList to Container.
 *
 * Container must be an STL-compatible container having a push_back
 * method. The elements in the QVariantList must be convartable to
 * Container::value_type, otherwise ValueError will be thrown!
 */
template <typename Container>
Container fromQVariantList(QVariantList list)
{
    Container container;

    std::transform(
        list.cbegin(), list.cend(), std::back_inserter(container), fromQVariant<typename Container::value_type>);

    return container;
}

/**
 * Return the parameter as-is.
 *
 * This function is useful as a convert function, when no conversion
 * is necessary.
 */
template <typename T>
T verbatim(const T& v)
{
    return v;
}

/**
 * Convert a mapping-type Container to QVariantMap.
 *
 * For the conversion a `convertKey` and a `convertValue` function must
 * be supplied, where `convertKey` converts from Container::key_type to
 * QString and `convertValue converts from Container::mapping_type to
 * QVariant.
 */
template <typename Container, typename ConvertKeyFunc, typename ConvertValueFunc>
QVariantMap toQVariantMap(Container container, ConvertKeyFunc convertKey, ConvertValueFunc convertValue)
{
    QVariantMap map;

    for (const auto& element : container)
    {
        QString key = convertKey(element.first);
        QVariant value = convertValue(element.second);
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
template <typename Container, typename ConvertKeyFunc, typename ConvertValueFunc>
Container fromQVariantMap(QVariantMap map, ConvertKeyFunc convertKey, ConvertValueFunc convertValue)
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

} // namespace utils
} // namespace warmonger

#endif // CORE_QVARIANT_UTIL_H
