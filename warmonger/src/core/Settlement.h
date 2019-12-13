/** \file
 * Settlement class.
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

#ifndef W_CORE_SETTLEMENT_H
#define W_CORE_SETTLEMENT_H

#include <QString>

#include "core/IntermediateRepresentation.h"
#include "core/WObject.h"

namespace warmonger {
namespace core {

class Faction;
class Map;
class MapNode;

/**
 * A Settlement.
 */
class Settlement : public WObject, public ir::Serializable
{
    Q_OBJECT

    Q_PROPERTY(QString type READ getType NOTIFY typeChanged)
    Q_PROPERTY(MapNode* position READ getPosition NOTIFY positionChanged)
    Q_PROPERTY(Faction* faction READ getOwner NOTIFY ownerChanged)

public:
    explicit Settlement(QObject* parent = nullptr, ObjectId id = ObjectId::Invalid);

    /**
     * Construct the settlement from the intermediate-representation.
     *
     * Unserializing constructor.
     */
    Settlement(ir::Value v, Map& map, QObject* parent);

    ir::Value serialize() const override;

    const QString& getType() const
    {
        return this->type;
    }

    void setType(QString type);

    MapNode* getPosition() const
    {
        return this->position;
    }

    void setPosition(MapNode* mapNode);

    Faction* getOwner() const
    {
        return this->owner;
    }

    void setOwner(Faction* owner);

signals:
    void typeChanged();
    void positionChanged();
    void ownerChanged();

private:
    QString type;
    MapNode* position = nullptr;
    Faction* owner = nullptr;
};

} // namespace core
} // namespace warmonger

#endif // W_CORE_SETTLEMENT_H
