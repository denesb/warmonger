/**
 * \copyright (C) 2014-2016 Botond Dénes
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

#include "io/Reference.h"

#include "core/Map.h"
#include "utils/Logging.h"

namespace warmonger {
namespace io {

static std::tuple<QString, QString, core::ObjectId> parseReference(const QString& reference);
static core::WObject* unserializeReference(const QString& className, core::ObjectId id, QObject* parent);

QString serializeReference(core::WObject* obj)
{
    const QMetaObject* metaObj = obj->metaObject();

    QObject* root = core::getObjectTreeRoot(obj);

    if (root == nullptr)
        return QString(metaObj->className()) + "#" + QString::number(obj->getId().get());

    const QMetaObject* rootMetaObj = root->metaObject();

    return QString(rootMetaObj->className()) + "/" + QString(metaObj->className()) + "#" +
        QString::number(obj->getId().get());
}

core::WObject* unserializeReference(const QString& reference, core::World* world)
{
    if (world == nullptr)
        return nullptr;

    QString parentClassName;
    QString objectClassName;
    core::ObjectId id;

    std::tie(parentClassName, objectClassName, id) = parseReference(reference);

    if (parentClassName.isEmpty() || objectClassName.isEmpty() || id == core::ObjectId::Invalid)
        return nullptr;

    if (parentClassName != world->metaObject()->className())
    {
        wWarning << "Unknown parent type found: " << parentClassName;
        return nullptr;
    }

    return unserializeReference(objectClassName, id, world);
}

core::WObject* unserializeReference(const QString& reference, core::Map* map)
{
    if (map == nullptr)
        return nullptr;

    QString parentClassName;
    QString objectClassName;
    core::ObjectId id;

    std::tie(parentClassName, objectClassName, id) = parseReference(reference);

    if (parentClassName.isEmpty() || objectClassName.isEmpty() || id == core::ObjectId::Invalid)
        return nullptr;

    if (parentClassName == map->metaObject()->className())
    {
        return unserializeReference(objectClassName, id, map);
    }
    else if (parentClassName == map->getWorld()->metaObject()->className())
    {
        return unserializeReference(objectClassName, id, map->getWorld());
    }
    else
    {
        wWarning << "Unknown parent type found: " << parentClassName;
        return nullptr;
    }
}

core::WObject* unserializeReference(const QString& reference, QObject* parent)
{
    if (parent == nullptr)
        return nullptr;

    if (auto world = qobject_cast<core::World*>(parent))
        return unserializeReference(reference, world);

    if (auto map = qobject_cast<core::Map*>(parent))
        return unserializeReference(reference, map);

    return unserializeReference(reference, parent->parent());
}

bool isReference(const QString& str)
{
    QString parentClassName;
    QString objectClassName;
    core::ObjectId id;
    std::tie(parentClassName, objectClassName, id) = parseReference(str);

    return !!id;
}

static std::tuple<QString, QString, core::ObjectId> parseReference(const QString& reference)
{
    const QStringList objects{reference.split('/')};
    const QStringList parts{objects.back().split('#')};

    const QString parentClassName = objects.size() == 2 ? objects.front() : QString();
    const QString objectClassName = parts.size() == 2 ? parts.front() : QString();
    const QString idStr = parts.size() == 2 ? parts.back() : QString();

    if (objects.size() != 2 || parts.size() != 2 || objectClassName.isEmpty() || idStr.isEmpty())
    {
        return std::make_tuple(QString(), QString(), core::ObjectId());
    }
    else
    {
        bool ok{false};
        const core::ObjectId id{idStr.toInt(&ok)};

        if (ok)
            return std::make_tuple(parentClassName, objectClassName, id);
        else
            return std::make_tuple(parentClassName, objectClassName, core::ObjectId::Invalid);
    }
}

static core::WObject* unserializeReference(const QString& className, core::ObjectId id, QObject* parent)
{
    if (className.isEmpty() || id == core::ObjectId::Invalid || parent == nullptr)
        return nullptr;

    auto children{parent->findChildren<core::WObject*>()};
    auto it = std::find_if(children.cbegin(), children.cend(), [&className, id](auto child) {
        return className == child->metaObject()->className() && id == child->getId();
    });

    if (it == children.cend())
        return nullptr;
    else
        return *it;
}

} // namespace warmonger
} // namespace io
