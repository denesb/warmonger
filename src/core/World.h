/** \file
 * World class.
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

#ifndef W_CORE_WORLD_H
#define W_CORE_WORLD_H

#include <memory>
#include <vector>

#include <QObject>

#include "core/IntermediateRepresentation.h"
#include "core/WorldRules.h"

namespace warmonger {
namespace core {

class NamedType : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName CONSTANT)
public:
    NamedType(QString name, QObject* parent)
        : QObject(parent)
        , name(name)
    {
        this->setObjectName(name);
    }

    QString getName() const
    {
        return this->name;
    }

private:
    QString name;
};

/**
 * A civilization.
 */
class Civilization : public NamedType
{
    Q_OBJECT

public:
    using NamedType::NamedType;
};

/**
 * An abstract faction banner.
 *
 * Surfaces define the actual look of the banner.
 */
class Banner : public NamedType
{
    Q_OBJECT

public:
    using NamedType::NamedType;
};

/**
 * Abstract color, used for the banners.
 *
 * Surfaces can define concrete colors that are mapped to the abstract
 * colors defined by the world.
 */
class Color : public NamedType
{
    Q_OBJECT

public:
    using NamedType::NamedType;
};

/**
 * Defines a game world.
 *
 * A world defines the content all its behaviour besides the core game rules.
 */
class World : public QObject, public ir::Serializable
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName NOTIFY nameChanged)
    Q_PROPERTY(QString uuid READ getUuid CONSTANT)
    Q_PROPERTY(QVariantList banners READ readBanners NOTIFY bannersChanged)
    Q_PROPERTY(QVariantList civilizations READ readCivilizations NOTIFY civilizationsChanged)
    Q_PROPERTY(QVariantList colors READ readColors NOTIFY colorsChanged)

public:
    /**
     * Constructs an empty world object.
     *
     * \param uuid the unique identifier of the world
     * \param rulesType the type of the rules
     * \param parent the parent QObject.
     */
    World(const QString& uuid, WorldRules::Type rulesType, QObject* parent = nullptr);

    /**
     * Construct the world from the intermediate-representation.
     *
     * Unserializing constructor.
     *
     * \param v the intermediate-representation
     * \param parent the parent QObject.
     */
    World(ir::Value v, QObject* parent = nullptr);

    ir::Value serialize() const override;

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
     * Get the rules entry point.
     *
     * The rules entry is an opaque handle to the file or function
     * (or other) which the world-rules use to load the rules.
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

    WorldRules::Type getRulesType() const
    {
        return this->rulesType;
    }

    /**
     * Load the rules from basePath.
     *
     * The base-path is used to resolve relative path within the rules,
     * including the entry point.
     * Should only be called when the world is set-up. Don't modify the
     * world afterwards.
     *
     * \param basePath the path to the world directory
     *
     * \throws IOError if the rules can't be loaded
     * \throws ValueError if the rules can't be parsed or initialization fails
     */
    void loadRules(const QString& basePath);

    WorldRules* getRules() const
    {
        return this->rules.get();
    }

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
     *
     * \return the creted banner
     */
    Banner* createBanner(QString name);

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
    Civilization* createCivilization(QString name);

    const std::vector<Color*>& getColors() const
    {
        return this->colors;
    }

    /**
     * Create a new color.
     *
     * The world takes ownership of the created object.
     * Will emit the signal World::colorsChanged().
     *
     * \returns the new color
     */
    Color* createColor(QString name);

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

signals:
    /**
     * Emitted when the name changes.
     */
    void nameChanged();

    /**
     * Emitted when the rules entry point changes.
     */
    void rulesEntryPointChanged();

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

private:
    QString uuid;
    QString name;
    QString rulesEntryPoint;
    WorldRules::Type rulesType;
    std::unique_ptr<WorldRules> rules;
    std::vector<Banner*> banners;
    std::vector<Civilization*> civilizations;
    std::vector<Color*> colors;
};

} // namespace core
} // namespace warmonger

#endif // W_CORE_WORLD_H
