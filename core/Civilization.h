/** \file
 * Civilization class.
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

#ifndef CORE_CIVILIZATION_H
#define CORE_CIVILIZATION_H

#include <map>
#include <vector>

#include <QObject>
#include <QVariant>

namespace warmonger {
namespace core {

class UnitType;

class Civilization : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(QVariantList unitTypes READ readUnitTypes NOTIFY unitTypesChanged)

public:
    explicit Civilization(QObject* parent = nullptr);

    QString getDisplayName() const;
    void setDisplayName(const QString& displayName);

    std::vector<UnitType*> getUnitTypes() const;
    QVariantList readUnitTypes() const;
    void setUnitTypes(const std::vector<UnitType*>& unitTypes);
    void addUnitType(UnitType* unitType);

signals:
    void displayNameChanged();
    void unitTypesChanged();

private:
    QString displayName;
    std::vector<UnitType*> unitTypes;
};

} // namespace core
} // namespace warmonger

#endif // CORE_CIVILIZATION_H
