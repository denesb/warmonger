/** \file
 * World class.
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

#ifndef W_CORE_WORLD_H
#define W_CORE_WORLD_H

#include <vector>

#include <QColor>
#include <QObject>
#include <QVariant>

#include "core/ArmyType.h"
#include "core/Banner.h"
#include "core/Civilization.h"
#include "core/SettlementType.h"
#include "core/TerrainType.h"
#include "core/UnitType.h"

namespace warmonger {
namespace core {

/**
 * Defines a game world.
 *
 * A world defines the content all its behaviour besides the core game rules.
 * It defines the available terrain, settlement, unit and army types and all
 * their properties.
 */
class World : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(QVariantList armyTypes READ readArmyTypes NOTIFY armyTypesChanged)
    Q_PROPERTY(QVariantList banners READ readBanners NOTIFY bannersChanged)
    Q_PROPERTY(QVariantList civilizations READ readCivilizations NOTIFY civilizationsChanged)
    Q_PROPERTY(QVariantList colors READ readColors NOTIFY colorsChanged)
    Q_PROPERTY(QVariantList settlementTypes READ readSettlementTypes NOTIFY settlementTypesChanged)
    Q_PROPERTY(QVariantList terrainTypes READ readTerrainTypes NOTIFY terrainTypesChanged)
    Q_PROPERTY(QVariantList unitTypes READ readUnitTypes NOTIFY unitTypesChanged)

public:
    /**
     * Constructs an empty world object.
     *
     *\param parent the parent QObject.
     */
    explicit World(QObject* parent = nullptr);

    /**
     * Get the display-name.
     *
     *\returns the displayName
     */
    const QString& getDisplayName() const
    {
        return this->displayName;
    }

    /**
     * Set the display-name.
     *
     * Will emit the signal World::displayNameChanged() if the newly set value
     * is different than the current one.
     *
     * \param displayName the new displayName
     */
    void setDisplayName(const QString& displayName);

    /**
     * Get the army-types.
     *
     * \return the army-types
     */
    const std::vector<ArmyType*>& getArmyTypes() const
    {
        return this->armyTypes;
    }

    /**
     * Set the army-types.
     *
     * Will emit the signal World::armyTypesChanged() if the newly set
     * value is different than the current one.
     *
     * \param army-types the new army-types
     */
    void setArmyTypes(const std::vector<ArmyType*>& armyTpes);

    /**
     * Get the army-types as a QVariantList.
     *
     * This function is used as a read function for the mapNodes property and is
     * not supposed to be called from C++ code. Use World::getArmyTypes()
     * instead.
     *
     * \returns the army-types
     */
    QVariantList readArmyTypes() const;

    /**
     * Get the banners.
     *
     * \return the banners
     */
    const std::vector<Banner*>& getBanners() const
    {
        return this->banners;
    }

    /**
     * Set the banners.
     *
     * Will emit the signal World::bannersChanged() if the newly set
     * value is different than the current one.
     *
     * \param banners the new banners
     */
    QVariantList readBanners() const;

    /**
     * Get the banners as a QVariantList.
     *
     * This function is used as a read function for the mapNodes property and is
     * not supposed to be called from C++ code. Use World::getBanners()
     * instead.
     *
     * \returns the banners
     */
    void setBanners(const std::vector<Banner*>& banners);

    /**
     * Get the civilizations.
     *
     * \return the civilizations
     */
    const std::vector<Civilization*>& getCivilizations() const
    {
        return this->civilizations;
    }

    /**
     * Set the civilizations.
     *
     * Will emit the signal World::civilizationsChanged() if the newly set
     * value is different than the current one.
     *
     * \param civilizations the new civilizations
     */
    void setCivilizations(const std::vector<Civilization*>& civilizations);

    /**
     * Get the civilizations as a QVariantList.
     *
     * This function is used as a read function for the mapNodes property and is
     * not supposed to be called from C++ code. Use World::getCivilizations()
     * instead.
     *
     * \returns the civilizations
     */
    QVariantList readCivilizations() const;

    /**
     * Get the colors.
     *
     * \return the colors
     */
    const std::vector<QColor>& getColors() const
    {
        return this->colors;
    }

    /**
     * Set the colors.
     *
     * Will emit the signal World::colorsChanged() if the newly set
     * value is different than the current one.
     *
     * \param colors the new colors
     */
    void setColors(const std::vector<QColor>& colors);

    /**
     * Get the colors as a QVariantList.
     *
     * This function is used as a read function for the mapNodes property and is
     * not supposed to be called from C++ code. Use World::getColors()
     * instead.
     *
     * \returns the colors
     */
    QVariantList readColors() const;

    /**
     * Get the settlement-types.
     *
     * \return the settlement-types
     */
    const std::vector<SettlementType*>& getSettlementTypes() const
    {
        return this->settlementTypes;
    }

    /**
     * Set the settlement-types.
     *
     * Will emit the signal World::settlementTypesChanged() if the newly set
     * value is different than the current one.
     *
     * \param settlement-types the new settlement-types
     */
    void setSettlementTypes(const std::vector<SettlementType*>& settlementTypes);

    /**
     * Get the settlement-types as a QVariantList.
     *
     * This function is used as a read function for the mapNodes property and is
     * not supposed to be called from C++ code. Use World::getSettlementTypes()
     * instead.
     *
     * \returns the settlement-types
     */
    QVariantList readSettlementTypes() const;

    /**
     * Get the terrain-types.
     *
     * \return the terrain-types
     */
    const std::vector<TerrainType*>& getTerrainTypes() const
    {
        return this->terrainTypes;
    }

    /**
     * Set the terrain-types.
     *
     * Will emit the signal World::terrainTypesChanged() if the newly set
     * value is different than the current one.
     *
     * \param terrain-types the new terrain-types
     */
    void setTerrainTypes(const std::vector<TerrainType*>& terrainTypes);

    /**
     * Get the terrain-types as a QVariantList.
     *
     * This function is used as a read function for the mapNodes property and is
     * not supposed to be called from C++ code. Use World::getTerrainTypes()
     * instead.
     *
     * \returns the terrain-types
     */
    QVariantList readTerrainTypes() const;

    /**
     * Get the unit-types.
     *
     * \return the unit-types
     */
    const std::vector<UnitType*>& getUnitTypes() const
    {
        return this->unitTypes;
    }

    /**
     * Set the unit-types.
     *
     * Will emit the signal World::unitTypesChanged() if the newly set
     * value is different than the current one.
     *
     * \param unit-types the new unit-types
     */
    void setUnitTypes(const std::vector<UnitType*>& unitTypes);

    /**
     * Get the unit-types as a QVariantList.
     *
     * This function is used as a read function for the mapNodes property and is
     * not supposed to be called from C++ code. Use World::getUnitTypes()
     * instead.
     *
     * \returns the unit-types
     */
    QVariantList readUnitTypes() const;

signals:
    /**
     * Emitted when the display-name changes.
     */
    void displayNameChanged();

    /**
     * Emitted when the army-types change.
     */
    void armyTypesChanged();

    /**
     * Emitted when the banners change.
     */
    void bannersChanged();

    /**
     * Emitted when the civilizations change.
     */
    void civilizationsChanged();

    /**
     * Emitted when the colors change.
     */
    void colorsChanged();

    /**
     * Emitted when the terrain-types change.
     */
    void terrainTypesChanged();

    /**
     * Emitted when the unit-types change.
     */
    void unitTypesChanged();

    /**
     * Emitted when the settlement-types change.
     */
    void settlementTypesChanged();

private:
    QString displayName;
    std::vector<ArmyType*> armyTypes;
    std::vector<Banner*> banners;
    std::vector<Civilization*> civilizations;
    std::vector<QColor> colors;
    std::vector<TerrainType*> terrainTypes;
    std::vector<UnitType*> unitTypes;
    std::vector<SettlementType*> settlementTypes;
};

} // namespace core
} // namespace warmonger

#endif // W_CORE_WORLD_H
