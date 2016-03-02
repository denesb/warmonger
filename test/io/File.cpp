#include <QFile>

#include "io/File.h"
#include "io/JsonSerializer.h"
#include "io/JsonUnserializer.h"
#include "test/catch.hpp"
#include "test/Util.h"

void createWorldFile(const QString &path)
{
    const QPair<core::World *, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};

    io::JsonSerializer serializer;

    QFile file(path);
    file.open(QIODevice::WriteOnly);

    file.write(serializer.serializeWorld(world.get()));
}

void createMapFile(const QString &path)
{
    const QPair<core::Map *, QJsonObject> maps = makeMap();
    const std::unique_ptr<core::Map> map{maps.first};

    io::JsonSerializer serializer;

    QFile file(path);
    file.open(QIODevice::WriteOnly);

    file.write(serializer.serializeMap(map.get()));
}

TEST_CASE("World can be written to file", "[File]")
{
    const QPair<core::World *, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};

    const QString path("./write_world.json");
    io::JsonSerializer serializer;

    SECTION("writing World")
    {
        io::writeWorld(world.get(), path, &serializer);

        QFile file(path);
        REQUIRE(file.exists() == true);

        file.remove();
    }
}

TEST_CASE("World can be read from file", "[File]")
{
    const QString path("./read_world.json");
    createWorldFile(path);

    io::Context ctx;
    io::JsonUnserializer unserializer(ctx);

    SECTION("reading World")
    {
        core::World *world = io::readWorld(path, &unserializer);

        REQUIRE(world != nullptr);
    }

    QFile file(path);
    file.remove();
}

TEST_CASE("Map can be written to file", "[File]")
{
    const QPair<core::Map *, QJsonObject> maps = makeMap();
    const std::unique_ptr<core::Map> map{maps.first};

    const QString path("./write_map.json");
    io::JsonSerializer serializer;

    SECTION("writing Map")
    {
        io::writeMap(map.get(), path, &serializer);

        QFile file(path);
        REQUIRE(file.exists() == true);

        file.remove();
    }
}

TEST_CASE("Map can be read from file", "[File]")
{
    io::Context ctx;
    const QString path("./read_map.json");
    io::JsonUnserializer unserializer(ctx);

    createMapFile(path);

    SECTION("reading Map")
    {
        //core::Map *map = io::readMap(path, &unserializer);

        //REQUIRE(map != nullptr);
    }

    QFile file(path);
    file.remove();
}
