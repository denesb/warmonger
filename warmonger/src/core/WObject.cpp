/**
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

#include <algorithm>
#include <iostream>
#include <vector>

#include "core/WObject.h"

namespace warmonger {
namespace core {

static ObjectId generateId(WObject* obj);

const ObjectId ObjectId::Invalid{};

inline bool operator<(ObjectId a, ObjectId b)
{
    return a.get() < b.get();
}

WObject::WObject(QObject* parent, ObjectId objectId)
    : QObject(parent)
    , objectId(!objectId ? generateId(this) : objectId)
{
}

QObject* getObjectTreeRoot(WObject* obj)
{
    if (obj == nullptr)
        return nullptr;

    QObject* parent = obj->parent();
    WObject* wparent = qobject_cast<WObject*>(parent);

    if (parent == nullptr) // no parent
        return parent;
    else if (wparent == nullptr) // parent is not WObject
        return parent;
    else // parent is WObject
        return getObjectTreeRoot(wparent);
}

static ObjectId generateId(WObject* obj)
{
    QObject* root{getObjectTreeRoot(obj)};

    if (root == nullptr)
    {
        return ObjectId::Invalid;
    }

    auto siblings{root->findChildren<WObject*>()};

    auto it = std::remove(siblings.begin(), siblings.end(), obj);

    if (it != siblings.end())
        siblings.erase(it);

    std::vector<ObjectId> ids;
    std::transform(siblings.cbegin(), siblings.cend(), std::back_inserter(ids), [](const auto& sibling) {
        return sibling->getId();
    });

    if (ids.empty())
    {
        return ObjectId(0);
    }
    else
    {
        return ObjectId(std::max_element(ids.cbegin(), ids.cend())->get() + 1);
    }
}

} // namespace core
} // namespace warmonger
