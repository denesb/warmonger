/**
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

#include <QtQml>

#include "ui/Banner.h"
#include "ui/CampaignMapEditor.h"
#include "ui/CampaignMiniMap.h"
#include "ui/UI.h"
#include "utils/Constants.h"

namespace warmonger {
namespace ui {

void initUI()
{
    const char* const applicationName = utils::applicationName.toStdString().c_str();

    qmlRegisterType<Banner>(applicationName, 1, 0, "Banner");
    qmlRegisterType<CampaignMapEditor>(applicationName, 1, 0, "CampaignMapEditor");
    qmlRegisterType<CampaignMiniMap>(applicationName, 1, 0, "CampaignMiniMap");
}

} // namespace ui
} // namespace warmonger
