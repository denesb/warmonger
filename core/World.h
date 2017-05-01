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
#include "core/EntityType.h"
#include "core/WorldComponentType.h"

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
    Q_PROPERTY(QString uuid READ getUuid CONSTANT)
    Q_PROPERTY(QVariantList banners READ readBanners NOTIFY bannersChanged)
    Q_PROPERTY(QVariantList civilizations READ readCivilizations NOTIFY civilizationsChanged)
    Q_PROPERTY(QVariantList colors READ readColors NOTIFY colorsChanged)
    Q_PROPERTY(QVariantList entityTypes READ readEntityTypes NOTIFY entityTypesChanged)
    Q_PROPERTY(QVariantList componentTypes READ readComponentTypes NOTIFY componentTypesChanged)

public:
    /**
     * Constructs an empty world object.
     *
     * \param uuid the uuid of the world
     * \param parent the parent QObject.
     */
    explicit World(const QString& uuid, QObject* parent = nullptr);

    /**
     * Get the uuid.
     *
     * \return the uuid
     */
    const QString getUuid() const
    {
        return this->uuid;
    }

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
     * Create a new banner.
     *
     * The world takes ownership of the created object.
     * Will emit the signal World::bannersChanged().
     * An id value should only be passed when the factions is being
     * unserialized and it already has a priorly generated id.
     *
     * \param id the id
     *
     * \return the new banner
     */
    Banner* createBanner(int id = WObject::invalidId);

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
     * Create a new civilization.
     *
     * The world takes ownership of the created object.
     * Will emit the signal World::civilizationsChanged().
     *
     * \returns the new civilization
     */
    Civilization* createCivilization();

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
     * Create a new world component-type.
     *
     * The world takes ownership of the created object.
     * Will emit the signal World::componentTypesChanged().
     * An id value should only be passed when the factions is being
     * unserialized and it already has a priorly generated id.
     *
     * \param id the id
     *
     * \return the new component-type
     */
    WorldComponentType* createWorldComponentType(int id = WObject::invalidId);

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
     * Create a new entity-type.
     *
     * The world takes ownership of the created object.
     * Will emit the signal World::entityTypesChanged().
     *
     * \return the new entity-type
     */
    EntityType* createEntityType();

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
    QString uuid;
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
