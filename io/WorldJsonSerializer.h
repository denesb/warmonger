/** \file
 * World JSON serializer.
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

#ifndef W_IO_WORLD_JSON_SERIALIZER_H
#define W_IO_WORLD_JSON_SERIALIZER_H

#include <QJsonDocument>

#include "io/WorldSerializer.h"

namespace warmonger {
namespace io {

/**
 * JSON serializer.
 *
 * Serialize any core game-object to JSON.
 */
class WorldJsonSerializer : public WorldSerializer
{
public:
    /**
     * Construct a json-serializer object.
     *
     * The compactness of the produced data can be changed by the format param.
     * Use QJsonDocument::Indented to produce nice human-readable output or
     * QJsonDocument::Compact to produce a compact output. The default is
     * QJsonDocument::Indented.
     *
     * \param format the formatting method to use
     */
    WorldJsonSerializer(QJsonDocument::JsonFormat format = QJsonDocument::Indented);

    /**
     * Serialize an banner.
     *
     * \param obj the banner
     *
     * \return the raw serialized data
     */
    QByteArray serializeBanner(const core::Banner* const obj) const override;

    /**
     * Serialize an civilization.
     *
     * \param obj the civilization
     *
     * \return the raw serialized data
     */
    QByteArray serializeCivilization(const core::Civilization* const obj) const override;

    /**
     * Serialize an component-type.
     *
     * \param obj the component-type
     *
     * \return the raw serialized data
     */
    QByteArray serializeComponentType(const core::ComponentType* const obj) const override;

    /**
     * Serialize an entity-type.
     *
     * \param obj the entity-type
     *
     * \return the raw serialized data
     */
    QByteArray serializeEntityType(const core::EntityType* const obj) const override;

    /**
     * Serialize a world.
     *
     * \param obj the world
     *
     * \return the raw serialized data
     */
    QByteArray serializeWorld(const core::World* const obj) const override;

private:
    QJsonDocument::JsonFormat format;
};

} // namespace warmonger
} // namespace io

#endif // W_IO_WORLD_JSON_SERIALIZER_H
