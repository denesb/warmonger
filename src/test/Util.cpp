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

#include "test/Util.h"

#include "core/Banner.h"
#include "core/BuiltInComponentTypes.h"
#include "core/Civilization.h"
#include "core/EntityType.h"
#include "core/Hexagon.h"
#include "core/MapNode.h"
#include "core/Utils.h"
#include "core/WorldComponentType.h"
#include "io/Reference.h"
#include "utils/ToString.h"

using namespace warmonger;

static void createEveryFieldType(core::WorldComponentType* componentType, QJsonObject& jcomponentType);

template <class GameObject>
void setNames(GameObject obj, QJsonObject& jobj, int i = 0)
{
    static const QString nameTemplate{"%1_name_%2"};

    const QString name = nameTemplate.arg(obj->metaObject()->className()).arg(i);

    obj->setName(name);
    jobj["name"] = name;
}

std::pair<std::unique_ptr<core::World>, QJsonObject> makeWorld()
{
    const QString positionComponentType{core::PositionComponentType::staticMetaObject.className()};
    const QString editComponentType{core::EditComponentType::staticMetaObject.className()};

    auto world{std::make_unique<core::World>(
        "universaly-unique-id-0", std::map<QString, int>{{positionComponentType, 0}, {editComponentType, 1}})};
    QJsonObject jworld;

    jworld["uuid"] = "universaly-unique-id-0";
    jworld["builtInObjectIds"] = QJsonObject{{positionComponentType, 0}, {editComponentType, 1}};

    world->setRulesEntryPoint("rules.lua");
    jworld["rulesEntryPoint"] = "rules.lua";

    world->setRulesType(core::RulesType::Lua);
    jworld["rulesType"] = "Lua";

    setNames(world.get(), jworld, 0);

    // ComponentType
    auto componentType0 = world->createWorldComponentType();
    componentType0->setName("componentType0");

    auto componentType0Field0 = componentType0->createField();
    componentType0Field0->setName("intField");
    componentType0Field0->setType(std::make_unique<core::FieldTypes::Integer>());

    auto componentType0Field1 = componentType0->createField();
    componentType0Field1->setName("listField");
    componentType0Field1->setType(
        std::make_unique<core::FieldTypes::List>(std::make_unique<core::FieldTypes::String>()));

    QJsonObject jcomponentType0;
    jcomponentType0["id"] = componentType0->getId();
    jcomponentType0["name"] = "componentType0";
    jcomponentType0["fields"] = QJsonArray{QJsonObject{{"name", "intField"}, {"type", "Integer"}},
        QJsonObject{{"name", "listField"}, {"type", QJsonObject{{"id", "List"}, {"valueType", "String"}}}}};

    auto componentType1 = world->createWorldComponentType();
    componentType1->setName("componentType1");

    QJsonObject jcomponentType1;
    jcomponentType1["id"] = componentType1->getId();
    jcomponentType1["name"] = "componentType1";

    createEveryFieldType(componentType1, jcomponentType1);

    jworld["componentTypes"] = QJsonArray({jcomponentType0, jcomponentType1});

    // EntityType
    auto entityType0 = world->createEntityType();
    entityType0->setName("entityType0");
    entityType0->addComponentType(componentType0);
    entityType0->addComponentType(componentType1);

    QJsonObject jentityType0;
    jentityType0["id"] = entityType0->getId();
    jentityType0["name"] = "entityType0";
    jentityType0["componentTypes"] =
        QJsonArray{io::serializeReference(componentType0), io::serializeReference(componentType1)};

    auto entityType1 = world->createEntityType();
    entityType1->setName("entityType1");
    entityType1->addComponentType(componentType1);

    QJsonObject jentityType1;
    jentityType1["id"] = entityType1->getId();
    jentityType1["name"] = "entityType1";
    jentityType1["componentTypes"] = QJsonArray{io::serializeReference(componentType1)};

    jworld["entityTypes"] = QJsonArray({jentityType0, jentityType1});

    // Civilization
    auto civilization0 = world->createCivilization();
    QJsonObject jcivilization0;
    setNames(civilization0, jcivilization0, 0);

    jcivilization0["id"] = civilization0->getId();

    jworld["civilizations"] = QJsonArray({jcivilization0});

    // Banner
    auto banner0 = world->createBanner();
    QJsonObject jbanner0;
    setNames(banner0, jbanner0, 0);

    banner0->setCivilizations({civilization0});
    jbanner0["id"] = banner0->getId();
    jbanner0["civilizations"] = QJsonArray({io::serializeReference(civilization0)});

    auto banner1 = world->createBanner();
    QJsonObject jbanner1;
    setNames(banner1, jbanner1, 1);

    jbanner1["id"] = banner1->getId();
    jbanner1["civilizations"] = QJsonArray();

    jworld["banners"] = QJsonArray({jbanner0, jbanner1});

    // Color
    world->setColors({QColor("#000000"), QColor("#ffffff")});
    jworld["colors"] = QJsonArray({"#000000", "#ffffff"});

    return std::make_pair(std::move(world), jworld);
}

std::pair<std::unique_ptr<core::Map>, QJsonObject> makeMap()
{
    auto map{std::make_unique<core::Map>()};
    QJsonObject jmap;

    setNames(map.get(), jmap, 0);

    auto worlds{makeWorld()};
    core::World* world = worlds.first.release();
    world->setParent(map.get());

    map->setWorld(world);
    jmap["world"] = world->getUuid();

    // MapNodes
    auto mapNode0 = map->createMapNode();
    QJsonObject jmapNode0;
    jmapNode0["id"] = mapNode0->getId();

    auto mapNode1 = map->createMapNode();
    QJsonObject jmapNode1;
    jmapNode1["id"] = mapNode1->getId();

    // MapNode neighbours
    mapNode0->setNeighbour(core::Direction::West, mapNode1);
    mapNode1->setNeighbour(core::Direction::East, mapNode0);
    jmapNode0["neighbours"] = QJsonObject{{"West", io::serializeReference(mapNode1)},
        {"NorthWest", ""},
        {"NorthEast", ""},
        {"SouthEast", ""},
        {"SouthWest", ""},
        {"East", ""}};
    jmapNode1["neighbours"] = QJsonObject{{"West", ""},
        {"NorthWest", ""},
        {"NorthEast", ""},
        {"SouthEast", ""},
        {"SouthWest", ""},
        {"East", io::serializeReference(mapNode0)}};

    jmap["mapNodes"] = QJsonArray({jmapNode0, jmapNode1});

    // Factions
    auto faction0 = map->createFaction();
    QJsonObject jfaction0;
    jfaction0["id"] = faction0->getId();

    setNames(faction0, jfaction0, 0);

    QColor primaryColor = world->getColors()[0];
    faction0->setPrimaryColor(primaryColor);
    jfaction0["primaryColor"] = primaryColor.name();

    QColor secondaryColor = world->getColors()[1];
    faction0->setSecondaryColor(secondaryColor);
    jfaction0["secondaryColor"] = secondaryColor.name();

    core::Banner* banner0 = world->getBanners()[0];

    faction0->setBanner(banner0);
    jfaction0["banner"] = io::serializeReference(banner0);

    core::Civilization* civilization0 = world->getCivilizations()[0];

    faction0->setCivilization(civilization0);
    jfaction0["civilization"] = io::serializeReference(civilization0);

    jmap["factions"] = QJsonArray{jfaction0};

    // Entities
    auto entityType0 = world->getEntityTypes()[0];
    auto componentType0 = entityType0->getComponentTypes()[0];
    auto componentType1 = entityType0->getComponentTypes()[1];

    auto entity0 = map->createEntity();
    entity0->setType(entityType0);
    entity0->getComponent(componentType0)->setField("intField", 100);
    entity0->getComponent(componentType0)->setField("listField", QVariantList{QString{"str0"}, QString{"str1"}});
    entity0->getComponent(componentType1)->setField("intField", 200);
    entity0->getComponent(componentType1)->setField("realField", 20.12);
    entity0->getComponent(componentType1)->setField("strField", "someStr");
    entity0->getComponent(componentType1)->setField("refField", QVariant::fromValue(faction0));
    entity0->getComponent(componentType1)->setField("intsListField", QVariantList{1, 5, 3});
    entity0->getComponent(componentType1)->setField("realMapField", QVariantMap{{"key0", 1.2}, {"key1", 3.3243}});
    entity0->getComponent(componentType1)
        ->setField("mapOfStrListsField",
            QVariantMap{
                {"banners", QVariantList{"banner0", "banner1"}}, {"civilizations", QVariantList{"civilization0"}}});

    QJsonObject jentity0;
    jentity0["id"] = entity0->getId();
    jentity0["type"] = io::serializeReference(entityType0);
    jentity0["components"] = QJsonObject{{io::serializeReference(componentType0),
                                             QJsonObject{{"intField", 100}, {"listField", QJsonArray{"str0", "str1"}}}},
        {io::serializeReference(componentType1),
            QJsonObject{{"intField", 200},
                {"realField", 20.12},
                {"strField", "someStr"},
                {"refField", io::serializeReference(faction0)},
                {"intsListField", QJsonArray{1, 5, 3}},
                {"realMapField", QJsonObject{{"key0", 1.2}, {"key1", 3.3243}}},
                {"mapOfStrListsField",
                    QJsonObject{{"banners", QJsonArray{"banner0", "banner1"}},
                        {"civilizations", QJsonArray{"civilization0"}}}}}}};

    jmap["entities"] = QJsonArray({jentity0});

    return std::make_pair(std::move(map), jmap);
}

static void createEveryFieldType(core::WorldComponentType* componentType, QJsonObject& jcomponentType)
{
    using namespace core::FieldTypes;

    auto intField = componentType->createField();
    intField->setName("intField");
    intField->setType(std::make_unique<Integer>());

    auto realField = componentType->createField();
    realField->setName("realField");
    realField->setType(std::make_unique<Real>());

    auto strField = componentType->createField();
    strField->setName("strField");
    strField->setType(std::make_unique<String>());

    auto refField = componentType->createField();
    refField->setName("refField");
    refField->setType(std::make_unique<Reference>());

    auto intsListField = componentType->createField();
    intsListField->setName("intsListField");
    intsListField->setType(std::make_unique<List>(std::make_unique<Integer>()));

    auto realMapField = componentType->createField();
    realMapField->setName("realMapField");
    realMapField->setType(std::make_unique<Map>(std::make_unique<Real>()));

    auto mapOfStrListsField = componentType->createField();
    mapOfStrListsField->setName("mapOfStrListsField");
    mapOfStrListsField->setType(std::make_unique<Map>(std::make_unique<List>(std::make_unique<String>())));

    jcomponentType["fields"] = QJsonArray{QJsonObject{{"name", "intField"}, {"type", "Integer"}},
        QJsonObject{{"name", "realField"}, {"type", "Real"}},
        QJsonObject{{"name", "strField"}, {"type", "String"}},
        QJsonObject{{"name", "refField"}, {"type", "Reference"}},
        QJsonObject{{"name", "intsListField"}, {"type", QJsonObject{{"id", "List"}, {"valueType", "Integer"}}}},
        QJsonObject{{"name", "realMapField"}, {"type", QJsonObject{{"id", "Map"}, {"valueType", "Real"}}}},
        QJsonObject{{"name", "mapOfStrListsField"},
            {"type",
                QJsonObject{{"id", "Map"},
                    {"valueType", QJsonObject{QJsonObject{{"id", "List"}, {"valueType", "String"}}}}}}}};
}
