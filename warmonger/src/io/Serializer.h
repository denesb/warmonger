/** \file
 * Serializer interface.
 *
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

#ifndef W_IO_SERIALIZER_H
#define W_IO_SERIALIZER_H

#include <QByteArray>

#include "core/IntermediateRepresentation.h"

namespace warmonger {
namespace io {

class Serializer
{
public:
    virtual QByteArray serialize(core::ir::Value) const = 0;
    virtual core::ir::Value unserialize(const QByteArray&) const = 0;
};

} // namespace io
} // namespace warmonger

#endif // W_IO_SERIALIZER_H
