cmake_minimum_required(VERSION 2.8.12)

project(Warmonger)

option(BUILD_TESTS "Build unit test suite" OFF)

include_directories(${PROJECT_SOURCE_DIR})

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

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(
    SRC_FILES
    core/Armor.cpp
    core/Army.cpp
    core/CampaignMap.cpp
    core/Civilization.cpp
    core/DamageType.cpp
    core/Faction.cpp
    core/Game.cpp
    core/MapNode.cpp
    core/Settlement.cpp
    core/SettlementType.cpp
    core/TerrainType.cpp
    core/Unit.cpp
    core/UnitClass.cpp
    core/UnitLevel.cpp
    core/UnitType.cpp
    core/Weapon.cpp
    core/WeaponClass.cpp
    core/WeaponType.cpp
    core/World.cpp
    io/Context.cpp
    io/File.cpp
    io/JsonSerializer.cpp
    io/JsonUnserializer.cpp
    log/ConsoleHandler.cpp
    log/Filter.cpp
    log/Formatter.cpp
    log/Handler.cpp
    log/LogRecord.cpp
    log/LogStream.cpp
    log/Logger.cpp
    ui/ApplicationContext.cpp
    ui/CampaignMapEditor.cpp
    ui/MapUtil.cpp
    ui/MapWindow.cpp
    ui/MiniMap.cpp
    ui/WorldSurface.cpp
)

find_package(Qt5Core REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(KF5Archive REQUIRED)

add_executable(warmonger ${SRC_FILES} ui/Main.cpp ui/ui.qrc)

set(
    WARMONGER_LIBS
    Qt5::Core
    Qt5::Quick
    KF5::Archive
)

target_link_libraries(
    warmonger
    ${WARMONGER_LIBS}
)

if(BUILD_TESTS)
    set(
        TEST_SRC_FILES
        ${SRC_FILES}
        test/Main.cpp
        test/Util.cpp
        test/core/MapNode.cpp
        test/io/Context.cpp
        test/io/File.cpp
        test/io/JsonSerializer.cpp
        test/io/JsonUnserializer.cpp
        test/ui/MapWindow.cpp
        test/ui/WorldSurface.cpp
    )

    add_executable(test_warmonger ${TEST_SRC_FILES})

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

    target_link_libraries(
        test_warmonger
        ${WARMONGER_LIBS}
    )
endif()
