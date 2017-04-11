/** \file
 * World class.
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

#ifndef W_CORE_WORLD_H
#define W_CORE_WORLD_H

#include <memory>
#include <vector>

#include <QColor>
#include <QObject>
#include <QVariant>

#include "core/Banner.h"
#include "core/Civilization.h"
#include "core/ComponentType.h"
#include "core/EntityType.h"

namespace warmonger {
namespace core {

/**
 * Defines a game world.
 *
 * A world defines the content all its behaviour besides the core game rules.
 * It defines the available civilizations, entity-types and component-types.
 */
class World : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(QVariantList banners READ readBanners NOTIFY bannersChanged)
    Q_PROPERTY(QVariantList civilizations READ readCivilizations NOTIFY civilizationsChanged)
    Q_PROPERTY(QVariantList colors READ readColors NOTIFY colorsChanged)
    Q_PROPERTY(QVariantList entityTypes READ readEntityTypes NOTIFY entityTypesChanged)
    Q_PROPERTY(QVariantList componentTypes READ readComponentTypes NOTIFY componentTypesChanged)

public:
    /**
     * Constructs an empty world object.
     *
     * \param parent the parent QObject.
     */
    explicit World(QObject* parent = nullptr);

    /**
     * Get the display-name.
     *
     * \returns the displayName
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
     * Get the banners.
     *
     * \return the banners
     */
    const std::vector<Banner*>& getBanners() const
    {
        return this->banners;
    }

    /**
     * Get the banners as a QVariantList.
     *
     * This function is used as a read function for the mapNodes property and is
     * not supposed to be called from C++ code. Use World::getBanners()
     * instead.
     *
     * \returns the banners
     */
    QVariantList readBanners() const;

    /**
     * Add a banner.
     *
     * The world will take ownership over the object.
     * Will emit the signal World::bannersChanged().
     * The behaviour is undefined if the civilization is already owned by this
     * world.
     *
     * \param banners the new banners
     */
    void addBanner(std::unique_ptr<Banner>&& banner);

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
     * Add a civilizations.
     *
     * The world will take ownership over the object.
     * Will emit the signal World::civilizationsChanged().
     * The behaviour is undefined if the civilization is already owned by this
     * world.
     *
     * \param civilizations the new civilization
     */
    void addCivilization(std::unique_ptr<Civilization>&& civilization);

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
     * Get the component-types.
     *
     * \return the component-types
     */
    const std::vector<ComponentType*>& getComponentTypes() const
    {
        return this->componentTypes;
    }

    /**
     * Get the component-types as a QVariantList.
     *
     * This function is used as a read function for the mapNodes property and is
     * not supposed to be called from C++ code. Use World::getComponentTypes()
     * instead.
     *
     * \returns the component-types
     */
    QVariantList readComponentTypes() const;

    /**
     * Add the component-type.
     *
     * The world will take ownership over the object.
     * Will emit the signal World::componentTypesChanged().
     * The behaviour is undefined if the component-type is already owned by this
     * world.
     *
     * \param component-type the new component-type
     */
    void addComponentType(std::unique_ptr<ComponentType>&& componentType);

    /**
     * Get the entity-types.
     *
     * \return the entity-types
     */
    const std::vector<EntityType*>& getEntityTypes() const
    {
        return this->entityTypes;
    }

    /**
     * Add the entity-type.
     *
     * The world will take ownership over the object.
     * Will emit the signal World::entityTypesChanged().
     * The behaviour is undefined if the civilization is already owned by this
     * world.
     *
     * \param entity-type the new entity-type
     */
    void addEntityType(std::unique_ptr<EntityType>&& entityTpe);

    /**
     * Get the entity-types as a QVariantList.
     *
     * This function is used as a read function for the mapNodes property and is
     * not supposed to be called from C++ code. Use World::getArmyTypes()
     * instead.
     *
     * \returns the entity-types
     */
    QVariantList readEntityTypes() const;

signals:
    /**
     * Emitted when the display-name changes.
     */
    void displayNameChanged();

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
    void componentTypesChanged();

    /**
     * Emitted when the unit-types change.
     */
    void entityTypesChanged();

private:
    QString displayName;
    std::vector<Banner*> banners;
    std::vector<Civilization*> civilizations;
    std::vector<QColor> colors;
    std::vector<ComponentType*> componentTypes;
    std::vector<EntityType*> entityTypes;
};

} // namespace core
} // namespace warmonger

#endif // W_CORE_WORLD_H
