/** \file
 * WObject.
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
#ifndef W_CORE_WOBJECT_H
#define W_CORE_WOBJECT_H

#include <QObject>

namespace warmonger {
namespace core {

/**
 * The basic core object of the warmonger object tree.
 *
 * WObjects have an unique id which is either passed in to the constructor or
 * generated when the object is created. The id is unique in the
 * context of it's parent, i.e. no other sibling type will have the same id.
 * WObject forms the backbone of the inter-object reference system in warmonger,
 * providing persistent references that can be serialized/unserialized to/from
 * the disk or the network.
 * WObjects should not be exposed to client code before their id is generated
 * or assigned.
 * WObjects don't support reparenting!
 */
class WObject : public QObject
{
    Q_OBJECT

    /**
     * The id of the object.
     */
    Q_PROPERTY(long id READ getId NOTIFY idChanged)

public:
    /**
     * The value of invalid ids.
     */
    static const int invalidId;

    /**
     * Create a WObject.
     *
     * This constructor should be used when a new object is created and it's
     * parent is known. If an id is not passed a new one will be generated.
     *
     * \param parent the parent QObject
     * \param id the id
     */
    WObject(QObject* parent, int id = WObject::invalidId);

    /**
     * Get the id.
     *
     * If the id is WObject::invalidId then the object is not yet referencable.
     *
     * \return the id
     */
    int getId() const
    {
        return this->id;
    }

signals:
    /**
     * Emitted when the id changes.
     */
    void idChanged();

private:
    void onParentChanged();

    int id;
};

/**
 * Get the root object of the object tree obj is member of.
 *
 * The root of the tree is the first parent that is not a WObject.
 * If such parent is not found nullptr is returned.
 *
 * \param obj the object
 *
 * \returns the root object or nullptr if not found
 */
QObject* getObjectTreeRoot(WObject* obj);

} // namespace core
} // namespace warmonger

#endif // W_CORE_WOBJECT_H