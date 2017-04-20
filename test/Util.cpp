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
#include "core/Civilization.h"
#include "core/EntityType.h"
#include "core/Hexagon.h"
#include "core/MapNode.h"
#include "core/Utils.h"
#include "core/WorldComponentType.h"
#include "io/Reference.h"
#include "utils/ToString.h"

using namespace warmonger;

template <class GameObject>
void setNames(GameObject obj, QJsonObject& jobj, int i = 0)
{
    static const QString displayNameTemplate{"%1_displayName_%2"};

    const QMetaObject* mo = obj->metaObject();

    const QString objectName{core::createObjectName(obj, i)};
    const QString displayName = displayNameTemplate.arg(mo->className()).arg(i);

    obj->setObjectName(objectName);
    jobj["objectName"] = objectName;

    obj->setDisplayName(displayName);
    jobj["displayName"] = displayName;
}

std::pair<std::unique_ptr<core::World>, QJsonObject> makeWorld()
{
    auto world{std::make_unique<core::World>("universaly-unique-id-0")};
    QJsonObject jworld;

    setNames(world.get(), jworld, 0);

    // ComponentType
    auto componentType0 = world->createWorldComponentType(0);
    componentType0->setObjectName(core::createObjectName(componentType0, 0));
    componentType0->setName("componentType0");

    auto componentType0Field0 = componentType0->createField();
    componentType0Field0->setName("intField");
    componentType0Field0->setType(std::make_unique<core::FieldTypes::Integer>());

    auto componentType0Field1 = componentType0->createField();
    componentType0Field1->setName("listField");
    componentType0Field1->setType(std::make_unique<core::FieldTypes::List>(std::make_unique<core::FieldTypes::String>()));

    QJsonObject jcomponentType0;
    jcomponentType0["id"] = 0;
    jcomponentType0["name"] = "componentType0";
    jcomponentType0["fields"] = QJsonArray{
        QJsonObject{{"name", "intField"}, {"type", "Integer"}},
        QJsonObject{{"name", "listField"}, {"type", QJsonObject{{"id", "List"}, {"valueType", "String"}}}}
    };

    auto componentType1 = world->createWorldComponentType(0);
    componentType1->setObjectName(core::createObjectName(componentType1, 1));
    componentType1->setName("componentType1");
    auto componentType1Field0 = componentType0->createField();
    componentType0Field0->setName("realField");
    componentType0Field0->setType(std::make_unique<core::FieldTypes::Real>());

    QJsonObject jcomponentType1;
    jcomponentType1["id"] = 1;
    jcomponentType1["name"] = "componentType1";
    jcomponentType0["fields"] = QJsonArray{
        QJsonObject{{"name", "realField"}, {"type", "Real"}}
    };

    jworld["componentTypes"] = QJsonArray({jcomponentType0, jcomponentType1});

    // EntityType
    auto entityType0 = world->createEntityType();
    entityType0->setName("entityType0");
    entityType0->addComponentType(componentType0);

    QJsonObject jentityType0;
    jentityType0["id"] = 2;
    jentityType0["name"] = "entityType0";
    jentityType0["componentTypes"] = QJsonArray{io::serializeReference(componentType0)};

    auto entityType1 = world->createEntityType();
    entityType1->setObjectName(core::createObjectName(entityType1, 1));
    entityType1->setName("entityType1");
    entityType1->addComponentType(componentType1);

    QJsonObject jentityType1;
    jentityType1["id"] = 1;
    jentityType1["name"] = "entityType1";
    jentityType1["componentTypes"] = QJsonArray{io::serializeReference(componentType1)};

    jworld["entityTypes"] = QJsonArray({jentityType0, jentityType1});

    // Civilization
    auto civilization0 = world->createCivilization();
    QJsonObject jcivilization0;
    setNames(civilization0, jcivilization0, 0);

    jworld["civilizations"] = QJsonArray({jcivilization0});

    // Banner
    auto banner0 = world->createBanner();
    QJsonObject jbanner0;
    setNames(banner0, jbanner0, 0);

    banner0->setCivilizations({civilization0});
    jbanner0["civilizations"] = QJsonArray({io::serializeReference(civilization0)});

    auto banner1 = world->createBanner();
    QJsonObject jbanner1;
    setNames(banner1, jbanner1, 1);

    jworld["banners"] = QJsonArray({jbanner0, jbanner1});

    // Color
    world->setColors({QColor("#000000"), QColor("#ffffff")});
    jworld["colors"] = QJsonArray({"#000000", "#ffffff"});

    return std::make_pair(std::move(world), jworld);
}

std::pair<std::unique_ptr<core::CampaignMap>, QJsonObject> makeMap()
{
    auto map{std::make_unique<core::CampaignMap>()};
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

    auto mapNode1 = map->createMapNode();
    QJsonObject jmapNode1;

    // MapNode neighbours
    mapNode0->setNeighbour(core::Direction::West, mapNode1);
    mapNode1->setNeighbour(core::Direction::East, mapNode0);
    jmapNode0["neighbours"] = QJsonObject{
        {"West", io::serializeReference(mapNode1)},
        {"NorthWest", ""},
        {"NorthEast", ""},
        {"SouthEast", ""},
        {"SouthWest", ""},
        {"East", ""}};
    jmapNode1["neighbours"] = QJsonObject{
        {"West", ""},
        {"NorthWest", ""},
        {"NorthEast", ""},
        {"SouthEast", ""},
        {"SouthWest", ""},
        {"East", io::serializeReference(mapNode0)}};

    jmap["mapNodes"] = QJsonArray({jmapNode0, jmapNode1});

    // Factions
    auto faction0 = map->createFaction();
    QJsonObject jfaction0;

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

    jmap["factions"] = QJsonArray({jfaction0});

    // Entities
    auto entityType0 = world->getEntityTypes()[0];
    auto componentType0 = entityType0->getComponentTypes()[0];

    auto entity0 = map->createEntity();
    entity0->setType(entityType0);
    (*entity0)[componentType0]->setField("intField", 100);
    (*entity0)[componentType0]->setField("listField", QVariantList{QString{"str0"}, QString{"str1"}});

    QJsonObject jentity0;
    jentity0["type"] = io::serializeReference(entityType0);
    jentity0["components"] = QJsonObject{
        {io::serializeReference(componentType0), QJsonObject{{"intField", 100}, {"listField", QJsonArray{"str0", "str1"}}}}
    };

    jmap["entities"] = QJsonArray({jentity0});

    return std::make_pair(std::move(map), jmap);
}
