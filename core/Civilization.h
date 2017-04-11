/** \file
 * Civilization class.
 *
 * \copyright (C) 2015-2017 Botond Dénes
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

#ifndef CORE_CIVILIZATION_H
#define CORE_CIVILIZATION_H

#include "core/WObject.h"

namespace warmonger {
namespace core {

/**
 * A civilization.
 */
class Civilization : public WObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)

public:
    /**
     * Creates an empty civilization object with the given id.
     *
     * \param id the id of the object
     */
    explicit Civilization(long id);

    /**
     * Creates an empty civilization object with the given parent.
     *
     * \param parent the parent QObject.
     */
    explicit Civilization(QObject* parent = nullptr);

    /**
     * Get the display-name.
     *
     * \returns the displayName
     */
    QString getDisplayName() const;

    /**
     * Set the display-name.
     *
     * Will emit the signal World::displayNameChanged() if the newly set value
     * is different than the current one.
     *
     * \param displayName the new displayName
     */
    void setDisplayName(const QString& displayName);

signals:
    /**
     * Emitted when the display-name changes.
     */
    void displayNameChanged();

private:
    QString displayName;
};

} // namespace core
} // namespace warmonger

#endif // CORE_CIVILIZATION_H
