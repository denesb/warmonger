/**
 * Copyright (C) 2015-2016 Botond Dénes
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

#include "core/CampaignMap.h"
#include "core/World.h"
#include "io/File.h"
#include "utils/Exception.h"

namespace warmonger {
namespace io {

void writeWorld(const core::World* world, const QString& path, io::Serializer& serializer)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly))
    {
        throw utils::IOError(QString("Failed to open %1 for writing").arg(path));
    }

    file.write(serializer.serializeWorld(world));
}

std::unique_ptr<core::World> readWorld(const QString& path, io::Unserializer& unserializer)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        throw utils::IOError(QString("Failed to open %1 for reading").arg(path));
    }

    const QByteArray data = file.readAll();
    return unserializer.unserializeWorld(data);
}

void writeCampaignMap(const core::CampaignMap* campaignMap, const QString& path, io::Serializer& serializer)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly))
    {
        throw utils::IOError(QString("Failed to open %1 for writing").arg(path));
    }

    file.write(serializer.serializeCampaignMap(campaignMap));
}

std::unique_ptr<core::CampaignMap> readCampaignMap(const QString& path, io::Unserializer& unserializer)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        throw utils::IOError(QString("Failed to open %1 for reading").arg(path));
    }

    const QByteArray data = file.readAll();
    return unserializer.unserializeCampaignMap(data);
}

} // namespace warmonger
} // namespace io
