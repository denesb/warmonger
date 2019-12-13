/** \file
 * WObject.
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
#ifndef W_CORE_WOBJECT_H
#define W_CORE_WOBJECT_H

#include <QObject>

namespace warmonger {
namespace core {

/**
 * Unique id identifying an WObject.
 *
 * This is just a type-safe wrapper around an int.
 */
class ObjectId
{
public:
    static const ObjectId Invalid;

    /**
     * Constructs an invalid ObjectId.
     *
     * ObjectId{} == ObjectId::Invalid.
     */
    ObjectId()
        : id(-1)
    {
    }

    explicit ObjectId(int id)
        : id(id)
    {
    }

    int get() const
    {
        return this->id;
    }

    bool operator!()
    {
        return this->id < 0;
    }

private:
    int id = -1;
};

inline bool operator==(ObjectId a, ObjectId b)
{
    return a.get() == b.get();
}

inline bool operator!=(ObjectId a, ObjectId b)
{
    return a.get() != b.get();
}

inline std::ostream& operator<<(std::ostream& os, ObjectId id)
{
    os << id.get();
    return os;
}

/**
 * The basic core object of the warmonger object tree.
 *
 * WObjects have an unique id which is either passed in to the constructor or
 * generated when the object is created. The id is unique in the
 * context of it's parent, i.e. no other sibling type will have the same id.
 * WObject forms the backbone of the inter-object reference system in warmonger,
 * providing persistent references that can be serialized/unserialized to/from
 * the disk or the network.
 * WObjects don't support reparenting!
 */
class WObject : public QObject
{
    Q_OBJECT

    /**
     * The id of the object.
     */
    Q_PROPERTY(ObjectId id READ getId CONSTANT)

public:
    template <typename Visitor>
    static auto describe(Visitor&& visitor)
    {
        return visitor.template visitParent<QObject>().visitMember("id", &WObject::getId);
    }

    /**
     * Get the object id.
     */
    ObjectId getId() const
    {
        return this->objectId;
    }

protected:
    /**
     * Create a WObject.
     *
     * If an invalid objectId is passed a new one will be generated.
     * The id should be unique amongst all the children of the first
     * non-WObject parent.
     *
     * \param parent the parent QObject
     * \param objectId the unique id of this instance
     */
    WObject(QObject* parent, ObjectId objectId);

private:
    void onParentChanged();

    ObjectId objectId;
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
