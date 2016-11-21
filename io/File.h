/** \file
 * File-level convenience functions.
 */

#ifndef IO_FILE_H
#define IO_FILE_H

#include <memory>

#include "io/Serializer.h"
#include "io/Unserializer.h"

namespace warmonger {
namespace io {

/**
 * Write the world to path.
 *
 * Serialize and write the world to the file at path.
 *
 * \param world the world
 * \param path the path where the world will be saved
 * \param serializer the serializer used for serializing the world
 *
 * \throw utils::IOError if the file at path is not writeable
 */
void writeWorld(const core::World* world, const QString& path, io::Serializer& serializer);

/**
 * Read the world from path.
 *
 * Read and unserialize the world from the file at path.
 *
 * \param path the path to the world file
 * \param unserializer the unserializer used for unserializing the world
 *
 * \return the loaded world
 *
 * \throw utils::IOError if the file at path is not readable
 * \throw utils::ValueError if there is a problem with unserializing
 */
std::unique_ptr<core::World> readWorld(const QString& path, io::Unserializer& unserializer);

/**
 * Write the campaign-map to path.
 *
 * Serialize and write the campaign-map to the file at path.
 *
 * \param world the campaign-map
 * \param path the path where the campaign-map will be saved
 * \param serializer the serializer used for serializing the campaign-map
 *
 * \throw utils::IOError if the file at path is not writeable
 */
void writeCampaignMap(const core::CampaignMap* campaignMap, const QString& path, io::Serializer& serializer);

/**
 * Read the campaign-map from path.
 *
 * Read and unserialize the campaign-map from the file at path.
 *
 * \param path the path to the campaign-map file
 * \param unserializer the unserializer used for unserializing the campaign-map
 *
 * \return the loaded campaign-map
 *
 * \throw utils::IOError if the file at path is not readable
 * \throw utils::ValueError if there is a problem with unserializing
 */
std::unique_ptr<core::CampaignMap> readCampaignMap(const QString& path, io::Unserializer& unserializer);

} // namespace warmonger
} // namespace io

#endif // IO_FILE_H
