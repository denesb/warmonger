#ifndef TEST_UTIL_H
#define TEST_UTIL_H

#include <ostream>
#include <string>

#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QObject>
#include <QString>
#include <catch.hpp>

#include "core/Map.h"
#include "core/World.h"
#include "io/Reference.h"

using namespace warmonger;

std::ostream& operator<<(std::ostream& os, const QString& s);

class FileKeeper
{
public:
    FileKeeper() = default;

    FileKeeper(const QString& filename, QByteArray content)
        : f(std::make_unique<QFile>(filename))
    {
        f->open(QIODevice::WriteOnly);
        f->write(content);
        f->flush();
        f->close();
    }

    FileKeeper(FileKeeper&&) = default;
    FileKeeper& operator=(FileKeeper&&) = default;

    ~FileKeeper()
    {
        if (f)
            f->remove();
    }

private:
    std::unique_ptr<QFile> f;
};

std::tuple<std::unique_ptr<core::World>, QJsonObject, FileKeeper> makeWorld();
std::tuple<std::unique_ptr<core::Map>, std::unique_ptr<core::World>, QJsonObject, FileKeeper> makeMap();

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

#define REQUIRE_REFERENCES(array, list)                                                                                \
    REQUIRE(array.size() == list.size());                                                                              \
    for (size_t i = 0; i < list.size(); i++)                                                                           \
    {                                                                                                                  \
        REQUIRE(list[i] != nullptr);                                                                                   \
        REQUIRE(array[i] == ::warmonger::io::serializeReference(list[i]));                                             \
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
