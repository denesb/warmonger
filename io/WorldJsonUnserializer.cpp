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

#include "io/WorldJsonUnserializer.h"

#include <QJsonObject>

#include "core/EntityType.h"
#include "core/World.h"
#include "core/WorldComponentType.h"
#include "io/JsonUtils.h"
#include "utils/Exception.h"

namespace warmonger {
namespace io {

std::unique_ptr<core::World> WorldJsonUnserializer::unserializeWorld(const QByteArray&) const
{
    return std::unique_ptr<core::World>();
}

std::unique_ptr<core::EntityType> WorldJsonUnserializer::unserializeEntityType(
    const QByteArray& data, const std::vector<core::ComponentType*>& allComponentTypes) const
{
    QJsonDocument jdoc{parseJson(data)};
    QJsonObject jobj{jdoc.object()};

    const QString& name{jobj["name"].toString()};

    if (name.isNull() || name.isEmpty())
    {
        throw utils::ValueError("Failed to unserialize entity-type, it doesn't have a name property");
    }

    const QJsonArray jcomponentTypeNames(jobj["componentTypes"].toArray());

    if (jcomponentTypeNames.isEmpty())
    {
        throw utils::ValueError("Failed to unserialize entity-type " + name + ", it doesn't have any component-types");
    }

    std::vector<core::ComponentType*> componentTypes;

    for (const auto& jcomponentTypeName : jcomponentTypeNames)
    {
        const QString name{jcomponentTypeName.toString()};

        if (name.isEmpty())
        {
            throw utils::ValueError("Failed to unserialize entity-type " + name + ", it has an empty component-type");
        }

        const auto it{std::find_if(allComponentTypes.cbegin(),
            allComponentTypes.cend(),
            [&name](const auto& componentType) { return componentType->getName() == name; })};

        if (it != allComponentTypes.cend())
        {
            componentTypes.push_back(*it);
        }
        else
        {
            throw utils::ValueError(
                "Failed to unserialize entity-type " + name + ", it has a non-existent component-type " + name);
        }
    }

    return std::make_unique<core::EntityType>(name, componentTypes);
}

std::unique_ptr<core::ComponentType> WorldJsonUnserializer::unserializeComponentType(const QByteArray& data) const
{
    QJsonDocument jdoc{parseJson(data)};
    QJsonObject jobj{jdoc.object()};

    const QString& name{jobj["name"].toString()};

    if (name.isNull() || name.isEmpty())
    {
        throw utils::ValueError("Failed to unserialize component-type, it doesn't have a name property");
    }

    const QJsonArray jpropertyNames(jobj["propertyNames"].toArray());

    if (jpropertyNames.isEmpty())
    {
        throw utils::ValueError("Failed to unserialize component-type " + name + ", it doesn't have any properties");
    }

    std::vector<QString> propertyNames;

    for (const auto& jpropertyName : jpropertyNames)
    {
        const QString propertyName{jpropertyName.toString()};

        if (propertyName.isNull())
        {
            throw utils::ValueError("Failed to unserialize component-type " + name + ", it has an empty property name");
        }

        propertyNames.push_back(propertyName);
    }

    return std::make_unique<core::WorldComponentType>(name, propertyNames);
}

} // namespace warmonger
} // namespace io
