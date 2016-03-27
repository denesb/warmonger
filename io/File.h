#ifndef IO_FILE_H
#define IO_FILE_H

#include "io/Serializer.h"
#include "io/Unserializer.h"

namespace warmonger {
namespace io {

void writeWorld(const core::World *world, const QString &path, io::Serializer *serializer);

core::World * readWorld(const QString &path, io::Unserializer *unserializer);

void writeCampaignMap(const core::CampaignMap *campaignMap, const QString &path, io::Serializer *serializer);

core::CampaignMap * readCampaignMap(const QString &path, io::Unserializer *unserializer);

} // namespace warmonger
} // namespace io

#endif // IO_FILE_H
