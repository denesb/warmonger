#ifndef TEST_UTIL_H
#define TEST_UTIL_H

#include <string>
#include <ostream>

#include <QJsonArray>
#include <QJsonObject>
#include <QString>

#include "core/World.h"
#include "test/catch.hpp"

using namespace warmonger;

std::ostream &operator<<(std::ostream &os, const QString &s);

QPair<core::World *, QJsonObject> makeWorld();

/**
 * Compare a QJsonObject to a map.
 *
 * Looks like we have to have this in the direct context of a TEST_CASE
 * to get rid of annoying warnings about parantheses.
 */
#define objectEqualsMap(object, map) \
    REQUIRE(object.size() == map.size()); \
    for (auto key : map.keys()) \
    { \
        REQUIRE(key != nullptr); \
        REQUIRE(object[key->objectName()] == map[key]); \
    }

/**
 * Compare a QJsonArray to a list.
 *
 * Looks like we have to have this in the direct context of a TEST_CASE
 * to get rid of annoying warnings about parantheses.
 */
#define arrayEqualsList(array, list) \
    REQUIRE(array.size() == list.size()); \
    for (int i = 0; i < list.size(); i++) \
    { \
        REQUIRE(list[i] != nullptr); \
        REQUIRE(array[i] == list[i]->objectName()); \
    }

#endif // TEST_UTIL_H
