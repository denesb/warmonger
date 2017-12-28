/**
 * Copyright (C) 2015-2017 Botond Dénes
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

#include "core/WObject.h"

using namespace warmonger;

class TestWObject1 : public core::WObject
{
    Q_OBJECT
public:
    TestWObject1(QObject* parent, core::ObjectId id = core::ObjectId::Invalid);
};

class TestWObject2 : public core::WObject
{
    Q_OBJECT
public:
    TestWObject2(QObject* parent, core::ObjectId id = core::ObjectId::Invalid);
};
