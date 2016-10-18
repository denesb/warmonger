cmake_minimum_required(VERSION 2.8.12)

project(Warmonger)

option(BUILD_TESTS "Build unit test suite" OFF)
option(BUILD_TOOLS "Build development tools" OFF)

include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_BINARY_DIR})

add_compile_options(
    -std=c++14
    -Werror
    -Wpedantic
    -Wall
    -Wextra
    -Wchar-subscripts
    -Wcomment
    -Wreturn-type
    -Wswitch
    -Wsuggest-override
    -Wfatal-errors
)

configure_file(Version.h.in Version.h @ONLY)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(Boost_USE_STATIC_LIBS ON)

# Source files

set(
    UTILS_SRC_FILES
    utils/Logging.cpp
    utils/Settings.cpp
    utils/ToString.cpp
    utils/Utils.cpp
)

set(
    CORE_SRC_FILES
    core/Army.cpp
    core/ArmyType.cpp
    core/CampaignMap.cpp
    core/Civilization.cpp
    core/Faction.cpp
    core/Game.cpp
    core/Hexagon.cpp
    core/MapGenerator.cpp
    core/MapNode.cpp
    core/MapNodeNeighbours.cpp
    core/Settlement.cpp
    core/SettlementType.cpp
    core/TerrainType.cpp
    core/Unit.cpp
    core/UnitType.cpp
    core/World.cpp
)

set(
    IO_SRC_FILES
    io/Context.cpp
    io/File.cpp
    io/JsonSerializer.cpp
    io/JsonUnserializer.cpp
)

set(
    UI_SRC_FILES
    ui/BasicMap.cpp
    ui/BasicMiniMap.cpp
    ui/CampaignMapEditor.cpp
    ui/CampaignMapWatcher.cpp
    ui/CampaignMiniMap.cpp
    ui/Context.cpp
    ui/MapUtil.cpp
    ui/MapWindow.cpp
    ui/SearchPaths.cpp
    ui/WorldSurface.cpp
    ui/ui.qrc
)

set(
    TEST_SRC_FILES
    test/test_warmonger.cpp
    test/Util.cpp
    test/core/ArmyType.cpp
    test/core/CampaignMap.cpp
    test/core/HierarchyNode.cpp
    test/core/MapGenerator.cpp
    test/core/MapNodeNeighbours.cpp
    test/core/SettlementType.cpp
    test/core/TerrainType.cpp
    test/core/UnitType.cpp
    test/io/Context.cpp
    test/io/File.cpp
    test/io/JsonSerializer.cpp
    test/io/JsonUnserializer.cpp
    test/ui/MapUtil.cpp
    test/ui/MapWindow.cpp
    test/ui/WorldSurface.cpp
)

file(GLOB_RECURSE ALL_HEADER_FILES ${PROJECT_SOURCE_DIR}/*.h)
file(GLOB_RECURSE ALL_SOURCE_FILES ${PROJECT_SOURCE_DIR}/*.cpp)

# Libraries
find_package(Qt5Core REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(KF5Archive REQUIRED)
find_package(Boost REQUIRED COMPONENTS log)

set(
    COMMON_EXTERNAL_LIBS
    Qt5::Quick
    Qt5::Core
    Boost::log
)

# Targets

add_library(utils STATIC ${UTILS_SRC_FILES})
qt5_use_modules(utils Core Quick)

add_library(core STATIC ${CORE_SRC_FILES})
qt5_use_modules(core Core Quick)

add_library(io STATIC ${IO_SRC_FILES})
qt5_use_modules(io Core Quick)

add_library(ui STATIC ${UI_SRC_FILES})
qt5_use_modules(ui Core Quick)
target_link_libraries(ui KF5::Archive)

add_executable(
    warmonger_mapeditor
    warmonger_mapeditor/warmonger_mapeditor.cpp
    warmonger_mapeditor/warmonger_mapeditor.qrc
)

target_link_libraries(
    warmonger_mapeditor
    ui
    io
    core
    utils
    ${COMMON_EXTERNAL_LIBS}
)

if(BUILD_TOOLS)
    add_executable(
        warmonger_world_smoketest
        tools/warmonger_world_smoketest.cpp
    )

    target_link_libraries(
        warmonger_world_smoketest
        io
        core
        utils
        ${COMMON_EXTERNAL_LIBS}
    )
endif()

if(BUILD_TESTS)
    add_executable(
        test_warmonger
        ${TEST_SRC_FILES}
        test/test_warmonger.cpp
    )

    execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/test/data/dev.wsp ${CMAKE_BINARY_DIR})
    execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/test/data/dev_nometafile.wsp ${CMAKE_BINARY_DIR})
    execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/test/data/dev_metainvalidjson.wsp ${CMAKE_BINARY_DIR})

    execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/test/data/dev_metadir.wsp ${CMAKE_BINARY_DIR})
    execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/test/data/dev_norcc.wsp ${CMAKE_BINARY_DIR})
    execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/test/data/dev_rccdir.wsp ${CMAKE_BINARY_DIR})
    execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/test/data/dev_rccinvalid.wsp ${CMAKE_BINARY_DIR})
    execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/test/data/dev_rccnodefinition.wsp ${CMAKE_BINARY_DIR})
    execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/test/data/dev_rccdefinitioninvalidjson.wsp ${CMAKE_BINARY_DIR})
    execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/test/data/dev_rccnohexmask.wsp ${CMAKE_BINARY_DIR})
    execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/test/data/dev_missingrequiredimages.wsp ${CMAKE_BINARY_DIR})

    target_link_libraries(
        test_warmonger
        ui
        io
        core
        utils
        ${COMMON_EXTERNAL_LIBS}
    )
endif()

add_custom_target(
    format
    COMMAND clang-format -i -style=file ${ALL_HEADER_FILES}
    COMMAND clang-format -i -style=file ${ALL_SOURCE_FILES}
    COMMENT "Formatting source files"
)
