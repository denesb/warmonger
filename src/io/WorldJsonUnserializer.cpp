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
#include <QMetaEnum>

#include "core/World.h"
#include "core/WorldComponentType.h"
#include "io/JsonUnserializer.hpp"
#include "io/JsonUtils.h"
#include "io/Reference.h"
#include "utils/Exception.h"

namespace warmonger {
namespace io {

static core::Banner* bannerFromJson(const QJsonObject& jobj, core::World* world);
static core::Civilization* civilizationFromJson(const QJsonObject& jobj, core::World* world);
static core::WorldComponentType* worldComponentTypeFromJson(const QJsonObject& jobj, core::World* world);

core::Banner* WorldJsonUnserializer::unserializeBanner(const QByteArray& data, core::World* world) const
{
    QJsonDocument jdoc(parseJson(data));
    return bannerFromJson(jdoc.object(), world);
}

core::Civilization* WorldJsonUnserializer::unserializeCivilization(const QByteArray& data, core::World* world) const
{
    QJsonDocument jdoc(parseJson(data));
    return civilizationFromJson(jdoc.object(), world);
}

core::WorldComponentType* WorldJsonUnserializer::unserializeWorldComponentType(
    const QByteArray& data, core::World* world) const
{
    QJsonDocument jdoc(parseJson(data));
    return worldComponentTypeFromJson(jdoc.object(), world);
}

std::unique_ptr<core::World> WorldJsonUnserializer::unserializeWorld(const QByteArray& data) const
{
    QJsonDocument jdoc(parseJson(data));

    try
    {
        return unserializeFromJson<core::World>(jdoc.object(), nullptr);
    }
    catch (utils::ValueError& e)
    {
        throw utils::ValueError(e, "Failed to unserialize world");
    }
}

static core::Banner* bannerFromJson(const QJsonObject& jobj, core::World* world)
{
    try
    {
        return world->addBanner(unserializeFromJson<core::Banner>(jobj, world));
    }
    catch (utils::ValueError& e)
    {
        throw utils::ValueError(e, "Failed to unserialize banner");
    }
}

static core::Civilization* civilizationFromJson(const QJsonObject& jobj, core::World* world)
{
    try
    {
        return world->addCivilization(unserializeFromJson<core::Civilization>(jobj, world));
    }
    catch (utils::ValueError& e)
    {
        throw utils::ValueError(e, "Failed to unserialize civilization");
    }
}

static core::WorldComponentType* worldComponentTypeFromJson(const QJsonObject& jobj, core::World* world)
{
    try
    {
        return world->addWorldComponentType(unserializeFromJson<core::WorldComponentType>(jobj, world));
    }
    catch (utils::ValueError& e)
    {
        throw utils::ValueError(e, "Failed to unserialize world component-type");
    }
}

} // namespace warmonger
} // namespace io
