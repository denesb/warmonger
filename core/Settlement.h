/** \file
 * Settlement class.
 *
 * \copyright (C) 2015-2016 Botond Dénes
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

#ifndef CORE_SETTLEMENT_H
#define CORE_SETTLEMENT_H

#include <vector>

#include <QObject>

#include "core/Faction.h"
#include "core/MapNode.h"
#include "core/SettlementType.h"

namespace warmonger {
namespace core {

class Settlement : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(SettlementType* type READ getType WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(MapNode* mapNode READ getMapNode WRITE setMapNode NOTIFY mapNodeChanged)
    Q_PROPERTY(Faction* owner READ getOwner WRITE setOwner NOTIFY ownerChanged)

public:
    explicit Settlement(QObject* parent = nullptr);
    ~Settlement();

    const QString& getDisplayName() const
    {
        return this->displayName;
    }

    void setDisplayName(const QString& displayName);

    SettlementType* getType() const
    {
        return this->type;
    }

    void setType(SettlementType* type);

    MapNode* getMapNode() const
    {
        return this->mapNode;
    }

    void setMapNode(MapNode* mapNode);

    Faction* getOwner() const
    {
        return this->owner;
    }

    void setOwner(Faction* owner);

signals:
    void displayNameChanged();
    void typeChanged();
    void mapNodeChanged();
    void ownerChanged();

private:
    QString displayName;
    SettlementType* type;
    MapNode* mapNode;
    Faction* owner;
};

} // namespace core
} // namespace warmonger

#endif // CORE_SETTLEMENT_H
