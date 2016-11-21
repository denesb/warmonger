/** \file
 * TerrainType class.
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

#ifndef W_CORE_TERRAIN_TYPE_H
#define W_CORE_TERRAIN_TYPE_H

#include <boost/optional.hpp>

#include <QObject>

#include "core/HierarchyNode.hpp"

namespace warmonger {
namespace core {

class TerrainType : public QObject, public HierarchyNode<TerrainType>
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)

public:
    explicit TerrainType(QObject* parent = nullptr);

    QString getDisplayName() const;
    void setDisplayName(const QString& displayName);

signals:
    void displayNameChanged();

private:
    boost::optional<QString> displayName;
};

} // namespace core
} // namespace warmonger

#endif // W_CORE_TERRAIN_TYPE_H
