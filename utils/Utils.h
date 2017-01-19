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
 * Convenience function to build a path from it's components.
 *
 * Two component specialization.
 *
 * \param head the first path-component
 * \param tail the second path-component
 *
 * \return the constructed path
 */
template <typename Head, typename Tail>
Head makePath(const Head& head, const Tail& tail)
{
    return head + "/" + tail;
}

/**
 * Convenience function to build a path from it's components.
 *
 * This function will accepth any number of components.
 *
 * \param head the first path-component
 * \param components the rest of the components
 *
 * \return the constructed path
 */
template <typename Head, typename... Component>
Head makePath(const Head& head, const Component&... components)
{
    return head + "/" + makePath(components...);
}

/**
 * Make a full file-name from the file's base-name and extension.
 *
 * \param fileName the base-name of the file
 * \param extension the file extension
 *
 * \return the file-name
 */
QString makeFileName(const QString& fileName, const QString& extension);

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
 * Construct the world path for the world.
 *
 * The settings have to be initialized for this to work.
 *
 * \param worldName the name of the world
 *
 * \return the path to the world definition file
 */
QString worldPath(const QString& worldName);

/**
 * Construct the world-surface path for the world-surface.
 *
 * The settings have to be initialized for this to work.
 *
 * \param worldName the name of the world
 * \param worldSurfaceName the name of the world-surface
 *
 * \return the path to the world definition file
 */
QString worldSurfacePath(const QString& worldName, const QString& worldSurfaceName);

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

} // namespace utils
} // namespace warmonger

#endif // W_UTILS_H
