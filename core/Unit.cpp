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

#include "core/Unit.h"

using namespace warmonger::core;

Unit::Unit(QObject* parent)
    : QObject(parent)
    , type(nullptr)
    , mapNode(nullptr)
    , owner(nullptr)
    , experiencePoints(0)
    , hitPoints(0)
    , movementPoints(0)
{
}

Unit::~Unit()
{
}

QString Unit::getDisplayName() const
{
    return this->displayName;
}

void Unit::setDisplayName(const QString& displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
}

UnitType* Unit::getType() const
{
    return this->type;
}

void Unit::setType(UnitType* type)
{
    if (this->type != type)
    {
        const UnitType* oldType = this->type;
        this->type = type;

        this->onTypeChanged(oldType);
        emit typeChanged();
    }
}

MapNode* Unit::getMapNode() const
{
    return this->mapNode;
}

void Unit::setMapNode(MapNode* mapNode)
{
    if (this->mapNode != mapNode)
    {
        this->mapNode = mapNode;
        emit mapNodeChanged();
    }
}

Faction* Unit::getOwner() const
{
    return this->owner;
}

void Unit::setOwner(Faction* owner)
{
    if (this->owner != owner)
    {
        this->owner = owner;
        emit ownerChanged();
    }
}

int Unit::getExperiencePoints() const
{
    return this->experiencePoints;
}

void Unit::setExperiencePoints(int experiencePoints)
{
    if (this->experiencePoints != experiencePoints)
    {
        this->experiencePoints = experiencePoints;
        emit experiencePointsChanged();
    }
}

int Unit::getHitPoints() const
{
    return this->hitPoints;
}

void Unit::setHitPoints(int hitPoints)
{
    if (this->hitPoints != hitPoints)
    {
        this->hitPoints = hitPoints;
        emit hitPointsChanged();
    }
}

int Unit::getMovementPoints() const
{
    return this->movementPoints;
}

void Unit::setMovementPoints(int movementPoints)
{
    if (this->movementPoints != movementPoints)
    {
        this->movementPoints = movementPoints;
        emit movementPointsChanged();
    }
}

void Unit::onTypeChanged(const UnitType* oldUnitType)
{
    if (oldUnitType == nullptr)
    {
        this->hitPoints = this->type->getHitPoints();
    }
    else
    {
        int hpPercentage = this->hitPoints * 100 / oldUnitType->getHitPoints();
        this->hitPoints = (this->type->getHitPoints() * hpPercentage) / 100;
    }
}
