/** \file
 * Generic utility functions.
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

#ifndef W_UTILS_H
#define W_UTILS_H

#include <QObject>
#include <QString>

namespace warmonger {
namespace utils {

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

template <typename Head, typename Tail>
Head makePath(const Head& head, const Tail& tail)
{
    return head + "/" + tail;
}

template <typename Head, typename... Component>
Head makePath(const Head& head, const Component&... components)
{
    return head + "/" + makePath(components...);
}

QString makeFileName(const QString& fileName, const QString& extension);

struct DelayedQObjectDeleter
{
    constexpr DelayedQObjectDeleter() = default;

    void operator()(QObject* object) const
    {
        object->deleteLater();
    }
};

} // namespace utils
} // namespace warmonger

#endif // W_UTILS_H
