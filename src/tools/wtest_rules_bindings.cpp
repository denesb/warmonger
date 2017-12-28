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

#include <QFileInfo>
#include <QMetaEnum>
#include <backward.hpp>

#include "core/Map.h"
#include "core/World.h"
#include "utils/Exception.h"
#include "utils/Logging.h"

namespace backward {

backward::SignalHandling sh;

} // namespace backward

using namespace warmonger;

static void createEveryFieldType(core::WorldComponentType* componentType)
{
    using FT = core::Field::Type;

    auto intField = componentType->createField();
    intField->setName("intField");
    intField->setType(FT::Integer);

    auto realField = componentType->createField();
    realField->setName("realField");
    realField->setType(FT::Real);

    auto strField = componentType->createField();
    strField->setName("strField");
    strField->setType(FT::String);

    auto refField = componentType->createField();
    refField->setName("refField");
    refField->setType(FT::Reference);

    auto listField = componentType->createField();
    listField->setName("listField");
    listField->setType(FT::List);

    auto mapField = componentType->createField();
    mapField->setName("mapField");
    mapField->setType(FT::Map);
}

static core::WorldRules::Type rulesTypeFromString(const char* str)
{
    const QMetaEnum metaEnum{QMetaEnum::fromType<core::WorldRules::Type>()};
    bool ok{true};
    auto e = metaEnum.keyToValue(str, &ok);

    if (!ok)
        throw utils::ValueError("Failed to unserialize RulesType");

    return static_cast<core::WorldRules::Type>(e);
}

/**
 * A sandbox to play and test the rules bindings in.
 */
int main(int argc, char* const argv[])
{
    if (argc < 3)
    {
        std::cout << "Usage: wtest_rules_bindings rules_type /path/to/rules_file" << std::endl;
        return 1;
    }

    QFileInfo f(argv[2]);

    if (!f.exists())
    {
        std::cout << "Rules file `" << argv[2] << "' does not exists" << std::endl;
        return 1;
    }

    utils::initLogging();

    core::World world("test_world_uuid", rulesTypeFromString(argv[1]));

    world.setRulesEntryPoint(f.fileName());
    world.setName("Test World");

    // ComponentType
    auto componentType0 = world.createWorldComponentType();
    componentType0->setName("simple");

    auto componentType0Field0 = componentType0->createField();
    componentType0Field0->setName("intField");
    componentType0Field0->setType(core::Field::Type::Integer);

    auto componentType0Field1 = componentType0->createField();
    componentType0Field1->setName("listField");
    componentType0Field1->setType(core::Field::Type::List);

    auto componentType1 = world.createWorldComponentType();
    componentType1->setName("complex");

    createEveryFieldType(componentType1);

    // Civilization
    auto civilization0 = world.createCivilization();
    civilization0->setName("Civilization 0");

    // Banner
    auto banner0 = world.createBanner();
    banner0->setName("Banner 0");
    banner0->setCivilizations({civilization0});

    auto banner1 = world.createBanner();
    banner1->setName("Banner 1");

    // Color
    world.setColors({QColor("#000000"), QColor("#ffffff")});

    world.loadRules(f.canonicalPath());

    world.getRules()->generateMap(4);

    return 0;
}
