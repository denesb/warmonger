#include <QFile>

#include "io/File.h"
#include "io/JsonSerializer.h"
#include "io/JsonUnserializer.h"
#include "test/catch.hpp"
#include "test/Util.h"

void createWorldFile(const QString &path)
{
    const std::pair<core::World *, QJsonObject> worlds = makeWorld();
    const std::unique_ptr<core::World> world{worlds.first};

    io::JsonSerializer serializer;

    QFile file(path);
    file.open(QIODevice::WriteOnly);

    file.write(serializer.serializeWorld(world.get()));
}

void createCampaignMapFile(const QString &path)
{
    const std::pair<core::CampaignMap *, QJsonObject> campaignMaps = makeMap();
    const std::unique_ptr<core::CampaignMap> campaignMap{campaignMaps.first};

    io::JsonSerializer serializer;

    QFile file(path);
    file.open(QIODevice::WriteOnly);

    file.write(serializer.serializeCampaignMap(campaignMap.get()));
}

TEST_CASE("World can be written to file", "[File]")
{
    const std::pair<core::World *, QJsonObject> worlds = makeWorld();
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
    const std::pair<core::CampaignMap *, QJsonObject> campaignMaps = makeMap();
    const std::unique_ptr<core::CampaignMap> campaignMap{campaignMaps.first};

    const QString path("./write_campaignmap.json");
    io::JsonSerializer serializer;

    SECTION("writing Map")
    {
        io::writeCampaignMap(campaignMap.get(), path, &serializer);

        QFile file(path);
        REQUIRE(file.exists() == true);

        file.remove();
    }
}

void loadWorld(
    const QString &className,
    const QString &objectName,
    io::Context &ctx
)
{
    const std::pair<core::World *, QJsonObject> worlds = makeWorld();
    core::World *world{worlds.first};

    if (className == world->metaObject()->className() ||
            objectName == world->objectName())
    {
        ctx.add(world);
        const std::vector<core::TerrainType *> tts = world->getTerrainTypes();
        std::for_each(
            tts.cbegin(),
            tts.cend(),
            [&](core::TerrainType *o){ctx.add(o);}
        );
        const std::vector<core::Civilization *> cs = world->getCivilizations();
        std::for_each(
            cs.cbegin(),
            cs.cend(),
            [&](core::Civilization *o){ctx.add(o);}
        );
        const std::vector<core::SettlementType *> sts = world->getSettlementTypes();
        std::for_each(
            sts.cbegin(),
            sts.cend(),
            [&](core::SettlementType *o){ctx.add(o);}
        );
        const std::vector<core::UnitType *> uts = world->getUnitTypes();
        std::for_each(
            uts.cbegin(),
            uts.cend(),
            [&](core::UnitType *o){ctx.add(o);}
        );
    }
}

TEST_CASE("Map can be read from file", "[File]")
{
    io::Context ctx(loadWorld);
    const QString path("./read_campaignmap.json");
    io::JsonUnserializer unserializer(ctx);

    createCampaignMapFile(path);

    SECTION("reading CampaignMap")
    {
        core::CampaignMap *campaignMap = io::readCampaignMap(path, &unserializer);

        REQUIRE(campaignMap != nullptr);
        REQUIRE(campaignMap->getWorld() != nullptr);
    }

    QFile file(path);
    file.remove();
}
