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
#include "core/WorldComponentType.h"
#include "core/WorldRules.h"

namespace warmonger {
namespace core {

/**
 * Defines a game world.
 *
 * A world defines the content all its behaviour besides the core game rules.
 * It defines the available civilizations and component-types.
 */
class World : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName NOTIFY nameChanged)
    Q_PROPERTY(QString uuid READ getUuid CONSTANT)
    Q_PROPERTY(QVariantList banners READ readBanners NOTIFY bannersChanged)
    Q_PROPERTY(QVariantList civilizations READ readCivilizations NOTIFY civilizationsChanged)
    Q_PROPERTY(QVariantList colors READ readColors NOTIFY colorsChanged)
    Q_PROPERTY(QVariantList componentTypes READ readComponentTypes NOTIFY componentTypesChanged)

public:
    template <class Visitor>
    static auto describe(Visitor&& visitor)
    {
        return visitor.template visitParent<QObject>()
            .visitMember("name", &World::getName)
            .visitMember("uuid", &World::getUuid)
            .visitMember("builtInObjectIds", &World::getBuiltInObjectIds)
            .visitMember("rulesEntryPoint", &World::getRulesEntryPoint)
            .visitMember("rulesType", &World::getRulesType)
            .visitMember("banners", &World::getBanners)
            .visitMember("civilizations", &World::getCivilizations)
            .visitMember("colors", &World::getColors)
            .visitMember("componentTypes", &World::getWorldComponentTypes);
    }

    /**
     * Constructs an empty world object.
     *
     * \param uuid the uuid of the world
     * \param parent the parent QObject.
     */
    World(const QString& uuid,
        const std::map<QString, int>& builtInObjectIds = std::map<QString, int>(),
        QObject* parent = nullptr);

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
     * Get the name.
     *
     * \returns the name
     */
    const QString& getName() const
    {
        return this->name;
    }

    /**
     * Set the name.
     *
     * Will emit the signal World::nameChanged() if the newly set value
     * is different than the current one.
     *
     * \param name the new name
     */
    void setName(const QString& name);

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
     * Add a new banner to the world.
     *
     * The world must already own this banner, i.e. it must have been
     * created with the world as its parent.
     *
     * \returns the added banner
     */
    Banner* addBanner(std::unique_ptr<Banner> banner);

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
     * An id value should only be passed when the factions is being
     * unserialized and it already has a priorly generated id.
     *
     * \param id the id
     *
     * \returns the new civilization
     */
    Civilization* createCivilization(int id = WObject::invalidId);

    /**
     * Add a new civilization to the world.
     *
     * The world must already own this civilization, i.e. it must have
     * been created with the world as its parent.
     *
     * \returns the added civilization
     */
    Civilization* addCivilization(std::unique_ptr<Civilization> civilization);

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
     * Get the world component-types.
     *
     * This list excludes any built-in component-types.
     */
    std::vector<WorldComponentType*> getWorldComponentTypes() const;

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
     * Get the built-in object id mapping.
     *
     * This mapping is used to permanently pin a certain built-in object to
     * the id it's first given when created.
     *
     * \returns the id mapping
     */
    const std::map<QString, int>& getBuiltInObjectIds() const
    {
        return this->builtInObjectIds;
    }

    /**
     * Get the rules entry point.
     *
     * \return the entry point
     */
    QString getRulesEntryPoint() const
    {
        return this->rulesEntryPoint;
    }

    /**
     * Set the rules entry point.
     *
     * Will emit the signal World::rulesEntryPointChanged() if the newly set
     * value is different than the current one.
     *
     * \param rulesEntryPoint the new entry point
     */
    void setRulesEntryPoint(const QString& rulesEntryPoint);

    /**
     * Get the rules type.
     *
     * \return the type
     */
    WorldRules::Type getRulesType() const
    {
        return this->rulesType;
    }

    /**
     * Set the rules type.
     *
     * Will emit the signal World::rulesTypeChanged() if the newly set
     * value is different than the current one.
     *
     * \param rulesType the type
     */
    void setRulesType(const WorldRules::Type rulesType);

    /**
     * Load the rules from basePath.
     *
     * The base-path is used to resolve relative path within the rules,
     * including the entry point.
     *
     * \param basePath the basePath
     *
     * \throws IOError if the rules can't be loaded
     * \throws ValueError if the rules can't be parsed or initialization fails
     */
    void loadRules(const QString& basePath);

    /**
     * Get the world-rules.
     *
     * \returns the rules
     */
    WorldRules* getRules() const
    {
        return this->rules;
    }

signals:
    /**
     * Emitted when the name changes.
     */
    void nameChanged();

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
     * Emitted when the rules entry point changes.
     */
    void rulesEntryPointChanged();

    /**
     * Emitted when the rules type changes.
     */
    void rulesTypeChanged();

private:
    QString uuid;
    QString name;
    std::vector<Banner*> banners;
    std::vector<Civilization*> civilizations;
    std::vector<QColor> colors;
    std::vector<ComponentType*> componentTypes;
    std::map<QString, int> builtInObjectIds;
    WObject* dummy;
    QString rulesEntryPoint;
    WorldRules::Type rulesType;
    WorldRules* rules;
};

} // namespace core
} // namespace warmonger

#endif // W_CORE_WORLD_H
