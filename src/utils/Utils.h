/** \file
 * Generic utility functions.
 *
 * \copyright (C) 2015-2018 Botond Dénes
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

#ifndef W_UTILS_H
#define W_UTILS_H

#include <QMetaEnum>
#include <QObject>
#include <QString>
#include <fmt/format.h>

#include "utils/Exception.h"

namespace warmonger {
namespace utils {

/**
 * Filter functor which can be used to find a QObject by name.
 */
struct QObjectFinder
{
    QObjectFinder(const QString& name)
        : name(name)
    {
    }

    bool operator()(const QObject* obj)
    {
        return obj->objectName() == this->name;
    }

    QString name;
};

/**
 * RAII class the block signals in the constructor and unblocks in the
 * destructor.
 */
class SignalBlockGuard
{
public:
    SignalBlockGuard(QObject* const obj)
        : obj(obj)
    {
        obj->blockSignals(true);
    }

    ~SignalBlockGuard()
    {
        obj->blockSignals(false);
    }

private:
    QObject* const obj;
};

/**
 * Delayed QObject deleter functor.
 *
 * When invoked this functor will call QObject::deleteLater() on the passed-in
 * object.
 */
struct DelayedQObjectDeleter
{
    constexpr DelayedQObjectDeleter() = default;

    void operator()(QObject* object) const
    {
        object->deleteLater();
    }
};

/**
 * Construct the path of the world's maps directory.
 *
 * The settings have to be initialized for this to work.
 *
 * \param worldName the name of the world
 *
 * \return the path to the world's maps directory
 */
QString worldMapsPath(const QString& worldName);

template <typename Enum>
QString enumToString(Enum e)
{
    static_assert(std::is_enum<Enum>::value, "Enum type must be an enum");
    const QMetaEnum metaEnum{QMetaEnum::fromType<Enum>()};
    auto str = metaEnum.valueToKey(static_cast<int>(e));
    if (!str)
        throw utils::ValueError("Cannot serialize unknown enum value: " + QString::number(static_cast<int>(e)));
    return QString(str);
}

template <typename Enum>
Enum enumFromString(const QString& str)
{
    static_assert(std::is_enum<Enum>::value, "Enum type must be an enum");
    const QMetaEnum metaEnum{QMetaEnum::fromType<Enum>()};
    bool ok{true};
    auto e = metaEnum.keyToValue(str.toLocal8Bit().data(), &ok);

    if (!ok)
        throw utils::ValueError(fmt::format(
            "Failed to unserialize enum: value `{}' does not correspont to any of the member values of `{}'",
            str,
            metaEnum.name()));

    return static_cast<Enum>(e);
}

} // namespace utils
} // namespace warmonger

#endif // W_UTILS_H
