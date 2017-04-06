/**
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

#include <algorithm>
#include <vector>
#include <iostream>


#include "core/WObject.h"

namespace warmonger {
namespace core {

static QObject* getObjectTreeRoot(QObject* obj);
static long generateId(QObject* obj);

const long WObject::invalidId{-1};

WObject::WObject(long id)
    : QObject()
    , id(id)
{
}

WObject::WObject(QObject* parent)
    : QObject(parent)
    , id(generateId(this))
{
}

static QObject* getObjectTreeRoot(QObject* obj)
{
    if (obj == nullptr)
        return nullptr;

    WObject* wobj{qobject_cast<WObject*>(obj)};

    if (wobj == nullptr)
    {
        return obj;
    }
    else
    {
        return getObjectTreeRoot(obj->parent());
    }
}

static long generateId(QObject* obj)
{
    QObject* root{getObjectTreeRoot(obj->parent())};

    if (root == nullptr)
    {
        return WObject::invalidId;
    }

    auto siblings{root->findChildren<WObject*>()};

    auto it = std::remove(siblings.begin(), siblings.end(), obj);

    if (it != siblings.end())
        siblings.erase(it);

    std::vector<long> ids;
    std::transform(siblings.cbegin(), siblings.cend(), std::back_inserter(ids), [](const auto& sibling){ return sibling->getId(); });

    if(ids.empty())
    {
        return 0;
    }
    else
    {
        return *std::max_element(ids.cbegin(), ids.cend()) + 1;
    }
}

} // namespace core
} // namespace warmonger
