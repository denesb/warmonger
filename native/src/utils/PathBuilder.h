/** \file
 * PathBuilder class.
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

#ifndef W_UTILS_PATH_BUILDER_H
#define W_UTILS_PATH_BUILDER_H

#include <QDir>
#include <QString>

namespace warmonger {
namespace utils {

/**
 * Allows conveniently building a path via operator/().
 *
 * Can be implicitly created from QDir or a QString and can be
 * implicitely converted to QString.
 * Does not validate the built paths.
 *
 * Example:
 *
 *      QFile f(dir / "worlds" / worldName / "world.wwd");
 */
class PathBuilder
{
public:
    /**
     * Create from the canonicalPath() of the dir.
     */
    PathBuilder(const QDir& dir)
        : path(dir.canonicalPath())
    {
    }

    PathBuilder(QString path)
        : path(std::move(path))
    {
    }

    operator QString() const
    {
        return this->path;
    }

    const QString& getPath() const
    {
        return this->path;
    }

    void append(const QString& path)
    {
        this->path += path;
    }

private:
    QString path;
};

} // namespace utils
} // namespace warmonger

warmonger::utils::PathBuilder operator/(warmonger::utils::PathBuilder pathBuilder, const QString& path);

#endif // W_UTILS_PATH_BUILDER_H
