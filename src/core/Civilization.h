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
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)

public:
    template <class Visitor>
    static auto describe(Visitor&& visitor)
    {
        return visitor.template visitParent<WObject>().visitMember(
            "name", &Civilization::getName, &Civilization::setName);
    }

    /**
     * Creates an empty civilization.
     *
     * \param parent the parent QObject.
     * \param id the id
     *
     * \see WObject::WObject
     */
    Civilization(QObject* parent, int id = WObject::invalidId);

    /**
     * Get the name.
     *
     * \returns the name
     */
    QString getName() const;

    /**
     * Set the name.
     *
     * Will emit the signal World::nameChanged() if the newly set value
     * is different than the current one.
     *
     * \param name the new name
     */
    void setName(const QString& name);

signals:
    /**
     * Emitted when the name changes.
     */
    void nameChanged();

private:
    QString name;
};

} // namespace core
} // namespace warmonger

#endif // CORE_CIVILIZATION_H
