/** \file
 * Persistent references.
 *
 * \copyright (C) 2014-2016 Botond Dénes
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

#ifndef W_IO_REFERENCE_H
#define W_IO_REFERENCE_H

#include <vector>

#include <QString>

class QObject;

namespace warmonger {

namespace core {
class CampaignMap;
class World;
class WObject;
}

namespace io {

/**
 * Serialize the reference to obj.
 *
 * Serialized references provide persistent inter-object links that survive
 * serializations. The serialized reference contains information about the
 * type of the parent, the type of the referenced object and the id of the
 * object.
 * The serialized reference has the format:
 * parent_type/object_type#object_id
 *
 * \param obj the object
 *
 * \return the serialized reference
 */
QString serializeReference(core::WObject* obj);

core::WObject* unserializeReference(const QString& reference, core::World* world);
core::WObject* unserializeReference(const QString& reference, core::CampaignMap* map);

} // namespace warmonger
} // namespace io

#endif // W_IO_REFERENCE_H

