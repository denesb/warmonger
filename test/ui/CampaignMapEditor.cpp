#include "ui/CampaignMapEditor.h"
#include "test/catch.hpp"

using namespace warmonger;

TEST_CASE("Invokable map-editing members", "[CampaignMapEditor]")
{
    core::World world;

    core::Civilization* civ = new core::Civilization();
    world.setCivilizations({civ});

    core::Banner* banner = new core::Banner();
    world.setBanners({banner});
    world.setColors({QColor("black"), QColor("white"), QColor("gray")});

    core::CampaignMap map;
    map.setWorld(&world);

    ui::WorldSurface surface("./test.wsp", &world);

    ui::CampaignMapEditor mapEditor;
    mapEditor.setCampaignMap(&map);
    mapEditor.setWorldSurface(&surface);

    SECTION("setNumberOfFactions")
    {
        REQUIRE(map.getFactions().empty());

        mapEditor.setNumberOfFactions(2);

        REQUIRE(map.getFactions().size() == 2);

        mapEditor.setNumberOfFactions(1);

        REQUIRE(map.getFactions().size() == 1);
    }
}
