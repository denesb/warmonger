/**
 * Copyright (C) 2015-2018 Botond Dénes
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

#include <QFile>
#include <QFileInfo>

#include "core/Map.h"
#include "core/World.h"
#include "io/File.h"
#include "io/JsonSerializer.h"
#include "utils/Constants.h"
#include "utils/Exception.h"
#include "utils/Format.h"
#include "utils/PathBuilder.h"
#include "utils/Settings.h"

namespace warmonger {
namespace io {

void writeWorld(const core::World* const world, const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly))
    {
        throw utils::IOError(QString("Failed to open %1 for writing").arg(path));
    }

    io::JsonSerializer serializer;

    file.write(serializer.serialize(world->serialize()));
}

std::unique_ptr<core::World> readWorld(const QString& path)
{
    auto file = [&] {
        auto file = std::make_unique<QFile>(path);

        if (file->exists())
            return file;

        const QString worldsPath = settingsValue(utils::SettingsKey::worldsDir).toString();
        file = std::make_unique<QFile>(worldsPath / path / path + "." + utils::fileExtensions::worldDefinition);

        if (!file->exists())
            throw utils::ValueError(
                fmt::format("Failed to read world: {} is not a known world name or a path to one", path));

        return file;
    }();

    if (!file->open(QIODevice::ReadOnly))
    {
        throw utils::IOError(QString("Failed to open %1 for reading").arg(path));
    }

    io::JsonSerializer serializer;

    auto world = std::make_unique<core::World>(serializer.unserialize(file->readAll()));

    QFileInfo fileInfo(*file);

    QString basePath;

    if (fileInfo.suffix() == utils::fileExtensions::worldDefinition)
    {
        basePath = fileInfo.canonicalPath();
    }
    else
    {
        basePath = ":/worldRules";
    }

    world->loadRules(basePath);

    return world;
}

void writeMap(const core::Map* const map, const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly))
    {
        throw utils::IOError(QString("Failed to open %1 for writing").arg(path));
    }

    io::JsonSerializer serializer;

    file.write(serializer.serialize(map->serialize()));
}

std::unique_ptr<core::Map> readMap(const QString& path, core::World* world)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        throw utils::IOError(QString("Failed to open %1 for reading").arg(path));
    }

    io::JsonSerializer serializer;

    return std::make_unique<core::Map>(serializer.unserialize(file.readAll()), *world, nullptr);
}

} // namespace io
} // namespace warmonger
