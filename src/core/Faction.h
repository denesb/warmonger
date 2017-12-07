/** \file
 * Faction class.
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

#ifndef CORE_FACTION_H
#define CORE_FACTION_H

#include <QColor>
#include <QString>

#include "core/Banner.h"
#include "core/Civilization.h"
#include "core/WObject.h"

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
class Faction : public WObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QColor primaryColor READ getPrimaryColor WRITE setPrimaryColor NOTIFY primaryColorChanged)
    Q_PROPERTY(QColor secondaryColor READ getSecondaryColor WRITE setSecondaryColor NOTIFY secondaryColorChanged)
    Q_PROPERTY(Banner* banner READ getBanner WRITE setBanner NOTIFY bannerChanged)
    Q_PROPERTY(Civilization* civilization READ getCivilization WRITE setCivilization NOTIFY civilizationChanged)

public:
    template <class Visitor>
    static auto describe(Visitor&& visitor)
    {
        return visitor.template visitParent<WObject>()
            .visitMember("name", &Faction::getName)
            .visitMember("primaryColor", &Faction::getPrimaryColor)
            .visitMember("secondaryColor", &Faction::getSecondaryColor)
            .visitMember("banner", &Faction::getBanner)
            .visitMember("civilization", &Faction::getCivilization);
    }

    /**
     * Construct an empty Faction.
     *
     * \param parent the parent QObject.
     * \param id the id
     *
     * \see WObject::WObject
     */
    Faction(QObject* parent, int id = WObject::invalidId);

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
     * \return the primary-color.
     *
     * \see Faction::setPrimaryColor()
     */
    const QColor& getPrimaryColor() const
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
    void setPrimaryColor(const QColor& primaryColor);

    /**
     * Get the secondary-color.
     *
     * \return the secondary-color.
     *
     * \see Faction::setSecondaryColor()
     */
    const QColor& getSecondaryColor() const
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
    void setSecondaryColor(const QColor& secondaryColor);

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
    QColor primaryColor;
    QColor secondaryColor;
    Banner* banner;
    Civilization* civilization;
};

} // namespace core
} // namespace warmonger

#endif // CORE_FACTION_H
