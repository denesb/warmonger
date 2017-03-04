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

#include "core/Utils.h"
#include "core/Banner.h"
#include "core/Civilization.h"
#include "core/Hexagon.h"
#include "core/MapNode.h"
#include "core/WorldComponentType.h"
#include "core/EntityType.h"
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

std::pair<core::World*, QJsonObject> makeWorld()
{
    core::World* w(new core::World());
    QJsonObject jw;

    setNames(w, jw, 0);

    // ComponentType
    auto ct0{std::make_unique<core::WorldComponentType>("componentType0", std::vector<QString>{"property0", "property1"})};
    ct0->setObjectName(core::createObjectName(ct0.get(), 0));

    QJsonObject jct0;
    jct0["id"] = 0;
    jct0["name"] = "componentType0";
    jct0["propertyNames"] = QJsonArray{"property0", "property1"};

    auto ct1{std::make_unique<core::WorldComponentType>("componentType1", std::vector<QString>{"property2", "property3"})};
    ct1->setObjectName(core::createObjectName(ct0.get(), 1));

    QJsonObject jct1;
    jct1["id"] = 1;
    jct1["name"] = "componentType1";
    jct1["propertyNames"] = QJsonArray{"property2", "property3"};

    w->addComponentType(std::move(ct0));
    w->addComponentType(std::move(ct1));
    jw["componentTypes"] = QJsonArray({jct0, jct1});

    // EntityType
    auto et0{std::make_unique<core::EntityType>("entityType0", std::vector<core::ComponentType*>{ct0.get()})};
    et0->setObjectName(core::createObjectName(et0.get(), 0));

    QJsonObject jet0;
    jet0["id"] = 0;
    jet0["name"] = "entityType0";
    jet0["type"] = "componentType:0";

    auto et1{std::make_unique<core::EntityType>("entityType1", std::vector<core::ComponentType*>{ct1.get()})};
    et1->setObjectName(core::createObjectName(et1.get(), 1));

    QJsonObject jet1;
    jet0["id"] = 1;
    jet0["name"] = "entityType1";
    jet0["type"] = "componentType:1";

    w->addEntityType(std::move(et0));
    w->addEntityType(std::move(et1));
    jw["entityTypes"] = QJsonArray({jet0, jet1});

    // Civilization
    auto c0{std::make_unique<core::Civilization>()};
    QJsonObject jc0;
    setNames(c0.get(), jc0, 0);

    w->addCivilization(std::move(c0));
    jw["civilizations"] = QJsonArray({jc0});

    // Banner
    auto b0{std::make_unique<core::Banner>()};
    QJsonObject jb0;
    setNames(b0.get(), jb0, 0);

    b0->setCivilizations({c0.get()});
    jb0["civilizations"] = QJsonArray({c0->objectName()});

    auto b1{std::make_unique<core::Banner>()};
    QJsonObject jb1;
    setNames(b1.get(), jb1, 1);

    w->addBanner(std::move(b0));
    w->addBanner(std::move(b1));
    jw["banners"] = QJsonArray({jb0, jb1});

    // Color
    w->setColors({QColor("#000000")});
    jw["colors"] = QJsonArray({"#000000"});

    return std::make_pair(w, jw);
}

std::pair<core::CampaignMap*, QJsonObject> makeMap()
{
    core::CampaignMap* m(new core::CampaignMap());
    QJsonObject jm;

    setNames(m, jm, 0);

    const std::pair<core::World*, QJsonObject> worlds = makeWorld();
    core::World* w = worlds.first;

    m->setWorld(w);
    jm["world"] = w->objectName();

    // MapNodes
    auto mn0{std::make_unique<core::MapNode>()};
    QJsonObject jmn0;

    //setNames(mn0.get(), jmn0, 0);

    auto mn1{std::make_unique<core::MapNode>()};
    QJsonObject jmn1;

    //setNames(mn1.get(), jmn1, 1);

    // MapNode neighbours
    mn0->setNeighbour(core::Direction::West, mn1.get());
    mn1->setNeighbour(core::Direction::East, mn0.get());
    jmn0["neighbours"] = QJsonObject{{"West", mn1->objectName()},
        {"NorthWest", ""},
        {"NorthEast", ""},
        {"SouthEast", ""},
        {"SouthWest", ""},
        {"East", ""}};
    jmn1["neighbours"] = QJsonObject{{"West", ""},
        {"NorthWest", ""},
        {"NorthEast", ""},
        {"SouthEast", ""},
        {"SouthWest", ""},
        {"East", mn0->objectName()}};

    m->addMapNode(std::move(mn0));
    m->addMapNode(std::move(mn1));
    jm["mapNodes"] = QJsonArray({jmn0, jmn1});

    // Factions
    auto f0{std::make_unique<core::Faction>()};
    QJsonObject jf0;

    setNames(f0.get(), jf0, 0);

    QColor primaryColor("red");
    f0->setPrimaryColor(primaryColor);
    jf0["primaryColor"] = primaryColor.name();

    QColor secondaryColor("black");
    f0->setSecondaryColor(secondaryColor);
    jf0["secondaryColor"] = secondaryColor.name();

    core::Banner* b0 = w->getBanners()[0];

    f0->setBanner(b0);
    jf0["banner"] = b0->objectName();

    core::Civilization* c0 = w->getCivilizations()[0];

    f0->setCivilization(c0);
    jf0["civilization"] = c0->objectName();

    m->addFaction(std::move(f0));
    jm["factions"] = QJsonArray({jf0});

    // Entitys
    auto e0{std::make_unique<core::Entity>(w->getEntityTypes()[0])};
    QJsonObject je0;

    je0["type"] = w->getEntityTypes()[0]->objectName();

    //(*e0.get())["property0"] = 100;
    //(*e0.get())["property1"] = "val1";
    je0["property0"] = 100;
    je0["property1"] = "val1";

    m->addEntity(std::move(e0));
    jm["entities"] = QJsonArray({je0});

    return std::make_pair(m, jm);
}
