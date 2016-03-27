#include <QFile>

#include "io/Exception.h"
#include "io/File.h"

namespace warmonger {
namespace io {

void writeWorld(const core::World *world, const QString &path, io::Serializer *serializer)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly))
    {
        throw FileIOError(QString("Failed to open %1 for writing").arg(path));
    }

    file.write(serializer->serializeWorld(world));
}

core::World * readWorld(const QString &path, io::Unserializer *unserializer)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        throw FileIOError(QString("Failed to open %1 for reading").arg(path));
    }

    const QByteArray data = file.readAll();
    return unserializer->unserializeWorld(data);
}

void writeCampaignMap(const core::CampaignMap *campaignMap, const QString &path, io::Serializer *serializer)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly))
    {
        throw FileIOError(QString("Failed to open %1 for writing").arg(path));
    }

    file.write(serializer->serializeCampaignMap(campaignMap));
}

core::CampaignMap * readCampaignMap(const QString &path, io::Unserializer *unserializer)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        throw FileIOError(QString("Failed to open %1 for reading").arg(path));
    }

    const QByteArray data = file.readAll();
    return unserializer->unserializeCampaignMap(data);
}

} // namespace warmonger
} // namespace io


