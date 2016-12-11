/**
 * Copyright (C) 2015-2016 Botond Dénes
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

#include "warmonger/Context.h"

#include <QGuiApplication>
#include <QStringList>

#include "io/File.h"
#include "io/JsonUnserializer.h"
#include "utils/Constants.h"
#include "utils/QVariantUtils.h"
#include "utils/Settings.h"
#include "utils/Utils.h"

namespace warmonger {

Context::Context(
    std::unique_ptr<core::World>&& world, std::unique_ptr<ui::WorldSurface>&& worldSurface, QObject* parent)
    : QObject(parent)
    , world(world.release())
    , worldSurface(worldSurface.release())
    , campaignMap(nullptr)
{
    this->world->setParent(this);
    this->worldSurface->setParent(this);
}

void Context::setCampaignMap(core::CampaignMap* campaignMap)
{
    if (campaignMap->getWorld() != this->world)
    {
        wWarning << "Cannot set campaign-map " << campaignMap << " with a different world";
        return;
    }

    if (this->campaignMap != campaignMap)
    {
        wInfo << "campaignMap: `" << this->campaignMap << "' -> `" << campaignMap << "'";

        this->campaignMap = campaignMap;

        this->campaignMap->setParent(this);

        emit campaignMapChanged();
    }
}

QVariantList Context::readCampaignMaps() const
{
    return utils::toQVariantList(this->campaignMaps);
}

} // namespace warmonger
