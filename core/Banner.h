/** \file
 * Banner class.
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

#ifndef W_CORE_BANNER_H
#define W_CORE_BANNER_H

#include <vector>

#include <QObject>

#include "core/Civilization.h"

namespace warmonger {
namespace core {

/**
 * A faction banner
 */
class Banner : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(QVariantList civilizations READ readCivilizations NOTIFY civilizationsChanged)

public:
    explicit Banner(QObject* parent = nullptr);

    const QString& getDisplayName() const
    {
        return this->displayName;
    }

    void setDisplayName(const QString& displayName);

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

    QVariantList readCivilizations() const;

    void setCivilizations(const std::vector<Civilization*>& civilizations);

signals:
    void displayNameChanged();
    void civilizationsChanged();

private:
    QString displayName;
    std::vector<Civilization*> civilizations;
};

} // namespace core
} // namespace warmonger

#endif // W_CORE_BANNER_H
