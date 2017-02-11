/** \file
 * SettlemenetType class.
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

#ifndef CORE_SETTLEMENT_TYPE_H
#define CORE_SETTLEMENT_TYPE_H

#include <vector>

#include <boost/optional.hpp>

#include <QObject>

#include "core/HierarchyNode.hpp"

namespace warmonger {
namespace core {

class UnitType;

/*
 * Can inherit:
 * * displayName
 */
class SettlementType : public QObject, public HierarchyNode<SettlementType>
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)

public:
    explicit SettlementType(QObject* parent = nullptr);

    QString getDisplayName() const;
    void setDisplayName(const QString& displayName);

signals:
    void displayNameChanged();

private:
    boost::optional<QString> displayName;
};

} // namespace core
} // namespace warmonger

#endif // CORE_SETTLEMENT_TYPE_H
