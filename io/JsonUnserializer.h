/** \file
 * JSON unserializer.
 *
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

#ifndef IO_JSON_UNSERIALIZER_H
#define IO_JSON_UNSERIALIZER_H

#include "io/Unserializer.h"

namespace warmonger {
namespace io {

/**
 * JSON unserializer.
 *
 * Unserialize any core game-object from JSON.
 */
class JsonUnserializer : public Unserializer
{
public:
    using Unserializer::Unserializer;

    /**
     * Unserialize an banner.
     *
     * \param data the raw data
     *
     * \return the unserialized banner object
     */
    std::unique_ptr<core::Banner> unserializeBanner(const QByteArray& data) override;

    /**
     * Unserialize an campaign-map.
     *
     * \param data the raw data
     *
     * \return the unserialized campaign-map object
     */
    std::unique_ptr<core::CampaignMap> unserializeCampaignMap(const QByteArray& data) override;

    /**
     * Unserialize an civilization.
     *
     * \param data the raw data
     *
     * \return the unserialized civilization object
     */
    std::unique_ptr<core::Civilization> unserializeCivilization(const QByteArray& data) override;

    /**
     * Unserialize an faction.
     *
     * \param data the raw data
     *
     * \return the unserialized faction object
     */
    std::unique_ptr<core::Faction> unserializeFaction(const QByteArray& data) override;

    /**
     * Unserialize an map-node.
     *
     * \param data the raw data
     *
     * \return the unserialized map-node object
     */
    std::unique_ptr<core::MapNode> unserializeMapNode(const QByteArray& data) override;

    /**
     * Unserialize a world.
     *
     * \param data the raw data
     *
     * \return the unserialized world object
     */
    std::unique_ptr<core::World> unserializeWorld(const QByteArray& data) override;
};

} // namespace warmonger
} // namespace io

#endif // IO_JSON_UNSERIALIZER_H
