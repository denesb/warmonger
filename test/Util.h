#ifndef TEST_UTIL_H
#define TEST_UTIL_H

#include <memory>
#include <string>
#include <ostream>

#include <QJsonArray>
#include <QJsonObject>
#include <QString>

#include "core/World.h"
#include "test/catch.hpp"

using namespace warmonger;

std::ostream &operator<<(std::ostream &os, const QString &s);

std::unique_ptr<core::World> makeWorld();

template <typename Map>
void objectEqualsMap(const QJsonObject &object, Map map)
{
    REQUIRE(object.size() == map.size());

    for (const typename Map::key_type key : map.keys())
    {
        REQUIRE(object[key->objectName()] == map[key]);
    }
}

template <typename List>
void arrayEqualsList(const QJsonArray &array, List list)
{
    REQUIRE(array.size() == list.size());

    for (int i = 0; i < list.size(); i++)
    {
        REQUIRE(array[i] == list[i]->objectName());
    }
}

#endif // TEST_UTIL_H
