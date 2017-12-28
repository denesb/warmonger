/** \file
 * Banner class.
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

#ifndef W_CORE_BANNER_H
#define W_CORE_BANNER_H

#include <vector>

#include <QVariant>

#include "core/Civilization.h"
#include "core/WObject.h"

namespace warmonger {
namespace core {

/**
 * A faction banner
 */
class Banner : public WObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName NOTIFY nameChanged)
    Q_PROPERTY(QVariantList civilizations READ readCivilizations NOTIFY civilizationsChanged)

public:
    template <class Visitor>
    static auto describe(Visitor&& visitor)
    {
        return visitor.template visitParent<WObject>()
            .visitMember("name", &Banner::getName, &Banner::setName)
            .visitMember("civilizations", &Banner::getCivilizations, &Banner::setCivilizations)
            .template visitConstructor<QObject*, ObjectId>("parent", "id");
    }

    /**
     * Construct an empty Banner.
     *
     * \param parent the parent QObject.
     * \param id the id
     *
     * \see WObject::WObject
     */
    Banner(QObject* parent, ObjectId id = ObjectId::Invalid);

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
     * Get the civilizations for this banner
     *
     * Banners can be restricted to certain civilizations. If the list is empty
     * the banner is not restricted to any civilizations.
     *
     * @returns const std::vector<Civilizations*>& the civilizations
     */
    const std::vector<Civilization*>& getCivilizations() const
    {
        return this->civilizations;
    }

    /**
     * Get the civilizations as a QVariantList.
     *
     * This function is used as a read function for the mapNodes property and is
     * not supposed to be called from C++ code. Use Banner::getCivilizations()
     * instead.
     *
     * \returns the banners
     */
    QVariantList readCivilizations() const;

    /**
     * Set the civilizations.
     *
     * Will emit the signal Banner::civilizationsChanged() if the newly set value
     * is different than the current one.
     *
     * \param civilizations the civilizations
     */
    void setCivilizations(const std::vector<Civilization*>& civilizations);

signals:
    /**
     * Emitted when the name changes.
     */
    void nameChanged();

    /**
     * Emitted when the civilizations change.
     */
    void civilizationsChanged();

private:
    QString name;
    std::vector<Civilization*> civilizations;
};

} // namespace core
} // namespace warmonger

#endif // W_CORE_BANNER_H
