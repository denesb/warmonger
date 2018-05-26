/** \file
 * Faction class.
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

#ifndef CORE_FACTION_H
#define CORE_FACTION_H

#include "core/World.h"

namespace warmonger {
namespace core {

/**
 * A faction represents a human or ai controlled player.
 *
 * It has a civilization, a banner, a primary and secondary color.
 *
 * \see Civilization
 * \see Banner
 */
class Faction : public WObject, public ir::Serializable
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(Color* primaryColor READ getPrimaryColor WRITE setPrimaryColor NOTIFY primaryColorChanged)
    Q_PROPERTY(Color* secondaryColor READ getSecondaryColor WRITE setSecondaryColor NOTIFY secondaryColorChanged)
    Q_PROPERTY(Banner* banner READ getBanner WRITE setBanner NOTIFY bannerChanged)
    Q_PROPERTY(Civilization* civilization READ getCivilization WRITE setCivilization NOTIFY civilizationChanged)

public:
    /**
     * Construct an empty Faction.
     *
     * \param parent the parent QObject.
     * \param id the id
     *
     * \see WObject::WObject
     */
    explicit Faction(QObject* parent = nullptr, ObjectId id = ObjectId::Invalid);

    /**
     * Construct the faction from the intermediate-representation.
     *
     * Unserializing constructor.
     *
     * \param v the intermediate-representation
     * \param parent the parent QObject.
     */
    Faction(ir::Value v, const World& world, QObject* parent);

    ir::Value serialize() const override;

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
     * Will emit the signal Faction::nameChanged() if the newly set value
     * is different than the current one.
     *
     * \param name the new name
     */
    void setName(const QString& name);

    /**
     * Get the primary-color.
     *
     * \see Faction::setPrimaryColor()
     */
    Color* getPrimaryColor() const
    {
        return this->primaryColor;
    }

    /**
     * Set the primary-color.
     *
     * The primary color is used (amongs others) as the foreground color of
     * the faction, \see Faction::setBanner().
     * Will emit the signal Faction::primaryColorChanged() if the newly set
     * value is different than the current one.
     *
     * \param the primary-color
     */
    void setPrimaryColor(Color* primaryColor);

    /**
     * Get the secondary-color.
     *
     * \see Faction::setSecondaryColor()
     */
    Color* getSecondaryColor() const
    {
        return this->secondaryColor;
    }

    /**
     * Set the secondary-color.
     *
     * The secondary color is used (amongs others) as the background color of
     * the faction, \see Faction::setBanner().
     * Will emit the signal Faction::secondaryColorChanged() if the newly set
     * value is different than the current one.
     *
     * \param the secondary-color
     */
    void setSecondaryColor(Color* secondaryColor);

    /**
     * Get the banner.
     *
     * \return the banner.
     *
     * \see Faction::setBanner()
     */
    Banner* getBanner() const
    {
        return this->banner;
    }

    /**
     * Set the banner.
     *
     * Then banner identifies the faction visually. It appears on every
     * settlement and army owned by the faction. It displays the factions
     * primary and secondary colors, the primary color as the foreground
     * and the secondary color as the background.
     * Will emit the signal Faction::bannerChanged() if the newly set value is
     * different than the current one.
     *
     * \param the banner
     *
     * \see setPrimaryColor()
     * \see setSecondaryColor()
     */
    void setBanner(Banner* banner);

    /**
     * Get the civilization.
     *
     * \return the civilization
     */
    Civilization* getCivilization() const
    {
        return this->civilization;
    }

    /**
     * Set the civilization.
     *
     * The civilization of a faction defines almost all of its aspects.
     * The units it can train, the armies it can raise, etc.
     * Will emit the signal Faction::civilizationChanged() if the newly set
     * value is different than the current one.
     *
     * \param the civilization
     */
    void setCivilization(Civilization* civilization);

signals:
    /**
     * Emitted when the name changes.
     */
    void nameChanged();

    /**
     * Emitted when the primary-color change.
     */
    void primaryColorChanged();

    /**
     * Emitted when the secondary-color change.
     */
    void secondaryColorChanged();

    /**
     * Emitted when the banner change.
     */
    void bannerChanged();

    /**
     * Emitted when the civilization change.
     */
    void civilizationChanged();

private:
    QString name;
    Color* primaryColor;
    Color* secondaryColor;
    Banner* banner;
    Civilization* civilization;
};

} // namespace core
} // namespace warmonger

#endif // CORE_FACTION_H
