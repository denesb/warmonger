#ifndef TEST_UTIL_H
#define TEST_UTIL_H

#include <ostream>
#include <string>

#include <QJsonArray>
#include <QJsonObject>
#include <QObject>
#include <QString>

#include "core/CampaignMap.h"
#include "core/World.h"
#include "test/catch.hpp"

using namespace warmonger;

std::ostream& operator<<(std::ostream& os, const QString& s);

std::pair<core::World*, QJsonObject> makeWorld();
std::pair<core::CampaignMap*, QJsonObject> makeMap();

/**
 * Compare a QJsonObject to a map.
 *
 * Looks like we have to have this in the direct context of a TEST_CASE
 * to get rid of annoying warnings about parantheses.
 */
#define objectEqualsMap(object, map)                                                                                   \
    REQUIRE(object.size() == map.size());                                                                              \
    for (const auto& element : map)                                                                                    \
    {                                                                                                                  \
        REQUIRE(element.first != nullptr);                                                                             \
        REQUIRE(object[element.first->objectName()] == element.second);                                                \
    }

/**
 * Compare a QJsonArray to a list.
 *
 * Looks like we have to have this in the direct context of a TEST_CASE
 * to get rid of annoying warnings about parantheses.
 */
#define arrayEqualsList(array, list)                                                                                   \
    REQUIRE(array.size() == list.size());                                                                              \
    for (size_t i = 0; i < list.size(); i++)                                                                           \
    {                                                                                                                  \
        REQUIRE(list[i] != nullptr);                                                                                   \
        REQUIRE(array[i] == list[i]->objectName());                                                                    \
    }

class DestroyWatcher : public QObject
{
    Q_OBJECT

public:
    DestroyWatcher(QObject* obj)
        : destroyed(false)
    {
        QObject::connect(obj, &QObject::destroyed, this, &DestroyWatcher::onDestroyed);
    }

    bool isDestroyed() const
    {
        return this->destroyed;
    }

public slots:
    void onDestroyed()
    {
        this->destroyed = true;
    }

private:
    bool destroyed;
};

#endif // TEST_UTIL_H
