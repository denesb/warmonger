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

#include "io/WorldJsonUnserializer.h"

#include "core/World.h"
#include "io/JsonUnserializer.hpp"

namespace warmonger {
namespace io {

core::Banner* WorldJsonUnserializer::unserializeBanner(const QByteArray& data, core::World* world) const
{
    try
    {
        return world->addBanner(unserializeFromJson<core::Banner>(parseJson(data).object(), world));
    }
    catch (utils::ValueError& e)
    {
        throw utils::ValueError(e, "Failed to unserialize banner");
    }
}

core::Civilization* WorldJsonUnserializer::unserializeCivilization(const QByteArray& data, core::World* world) const
{
    try
    {
        return world->addCivilization(unserializeFromJson<core::Civilization>(parseJson(data).object(), world));
    }
    catch (utils::ValueError& e)
    {
        throw utils::ValueError(e, "Failed to unserialize civilization");
    }
}

core::WorldComponentType* WorldJsonUnserializer::unserializeWorldComponentType(
    const QByteArray& data, core::World* world) const
{
    try
    {
        return world->addWorldComponentType(
            unserializeFromJson<core::WorldComponentType>(parseJson(data).object(), world));
    }
    catch (utils::ValueError& e)
    {
        throw utils::ValueError(e, "Failed to unserialize world component-type");
    }
}

std::unique_ptr<core::World> WorldJsonUnserializer::unserializeWorld(const QByteArray& data) const
{
    try
    {
        return unserializeFromJson<core::World>(parseJson(data).object(), nullptr);
    }
    catch (utils::ValueError& e)
    {
        throw utils::ValueError(e, "Failed to unserialize world");
    }
}

} // namespace warmonger
} // namespace io
