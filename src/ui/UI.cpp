/**
 * Copyright (C) 2015-2018 Botond Dénes
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
#include "ui/MapEditor.h"
#include "ui/MapView.h"
#include "ui/MiniMap.h"
#include "ui/UI.h"
#include "utils/Constants.h"

namespace warmonger {
namespace ui {

void initUI()
{
    const char* const applicationName = utils::applicationName.toStdString().c_str();

    qmlRegisterType<Banner>(applicationName, 1, 0, "Banner");
    qmlRegisterType<MapEditor>(applicationName, 1, 0, "MapEditor");
    qmlRegisterType<MapView>(applicationName, 1, 0, "MapView");
    qmlRegisterType<MiniMap>(applicationName, 1, 0, "MiniMap");
}

} // namespace ui
} // namespace warmonger
