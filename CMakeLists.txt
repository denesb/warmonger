#
# Copyright (C) 2015-2017 Botond Dénes
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#

cmake_minimum_required(VERSION 2.8.12)

project(Warmonger)

option(BUILD_TOOLS "Build development tools" OFF)
option(BUILD_TESTS "Build unit test suite" OFF)

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

find_program(GIT_PATH git DOC "Git executable path")

if(GIT_PATH)
    execute_process(
        COMMAND ${GIT_PATH} describe
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        RESULT_VARIABLE RESULT
        OUTPUT_VARIABLE VERSION_OUTPUT
    )
    string(STRIP "${VERSION_OUTPUT}" VERSION)

    execute_process(
        COMMAND ${GIT_PATH} show --pretty=format:%H -s
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_HASH_OUTPUT
    )
    string(STRIP "${GIT_COMMIT_HASH_OUTPUT}" GIT_COMMIT_HASH)
else()
    set(VERSION "")
    set(GIT_COMMIT_HASH "")
endif()

configure_file(Version.h.in Version.h @ONLY)

# AUTOMOC puts all moc files in the same directory
# this causes name clashes for warmonger and I'm not
# willing to make up new names for my files because
# of a limitiation in the build system.
# As a temporary workaround use WARMONGER_AUTOMOC(target)
# for each target whose source files possibly use Q_OBJECT
# or Q_GADGET
set(CMAKE_AUTOMOC OFF)
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
    core/Banner.cpp
    core/CampaignMap.cpp
    core/Civilization.cpp
    core/Faction.cpp
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
    io/SanityCheck.cpp
)

set(
    UI_SRC_FILES
    ui/Banner.cpp
    ui/BasicMap.cpp
    ui/BasicMiniMap.cpp
    ui/CampaignMapEditor.cpp
    ui/CampaignMapPreview.cpp
    ui/CampaignMapWatcher.cpp
    ui/CampaignMiniMap.cpp
    ui/MapUtil.cpp
    ui/MapWindow.cpp
    ui/Palette.cpp
    ui/SearchPaths.cpp
    ui/UI.cpp
    ui/WorldSurface.cpp
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
    test/io/SanityCheck.cpp
    test/ui/CampaignMapEditor.cpp
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

include(warmonger_automoc.cmake)

set(
    COMMON_EXTERNAL_LIBS
    Qt5::Quick
    Qt5::Core
    Boost::log
)

# Targets

add_library(utils STATIC ${UTILS_SRC_FILES})
qt5_use_modules(utils Core Quick)
WARMONGER_AUTOMOC(utils)

add_library(core STATIC ${CORE_SRC_FILES})
qt5_use_modules(core Core Quick)
WARMONGER_AUTOMOC(core)

add_library(io STATIC ${IO_SRC_FILES})
qt5_use_modules(io Core Quick)
WARMONGER_AUTOMOC(io)

add_library(ui STATIC ${UI_SRC_FILES})
qt5_use_modules(ui Core Quick)
target_link_libraries(ui KF5::Archive)
WARMONGER_AUTOMOC(ui)

add_executable(
    wmapeditor
    wmapeditor/Context.cpp
    wmapeditor/wmapeditor.cpp
    wmapeditor/wmapeditor.qrc
)
WARMONGER_AUTOMOC(wmapeditor)

target_link_libraries(
    wmapeditor
    ui
    io
    core
    utils
    ${COMMON_EXTERNAL_LIBS}
)

add_executable(
    warmonger
    warmonger/Context.cpp
    warmonger/warmonger.cpp
    warmonger/warmonger.qrc
)
WARMONGER_AUTOMOC(warmonger)

target_link_libraries(
    warmonger
    ui
    io
    core
    utils
    ${COMMON_EXTERNAL_LIBS}
)

if(BUILD_TOOLS)
    add_executable(
        wcheck_world
        tools/wcheck_world.cpp
        tools/Utils.cpp
    )

    target_link_libraries(
        wcheck_world
        ui
        io
        core
        utils
        ${COMMON_EXTERNAL_LIBS}
    )

    add_executable(
        wcheck_campaignmap
        tools/wcheck_campaignmap.cpp
        tools/Utils.cpp
    )

    target_link_libraries(
        wcheck_campaignmap
        ui
        io
        core
        utils
        ${COMMON_EXTERNAL_LIBS}
    )

    add_executable(
        wcheck_worldsurface
        tools/Utils.cpp
        tools/wcheck_worldsurface.cpp
    )

    target_link_libraries(
        wcheck_worldsurface
        ui
        io
        core
        utils
        ${COMMON_EXTERNAL_LIBS}
    )

    add_executable(
        wcreate_default_settings
        tools/wcreate_default_settings.cpp
    )

    target_link_libraries(
        wcreate_default_settings
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

    WARMONGER_AUTOMOC(test_warmonger)

    target_link_libraries(
        test_warmonger
        ui
        io
        core
        utils
        ${COMMON_EXTERNAL_LIBS}
    )

    set(TEST_DATA "${CMAKE_SOURCE_DIR}/test/data")
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${TEST_DATA}/worldsurface-packages
        ${CMAKE_BINARY_DIR}/worldsurface-packages
    )
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${TEST_DATA}/world-packages
        ${CMAKE_BINARY_DIR}/world-packages
    )
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${TEST_DATA}/campaignmap-packages
        ${CMAKE_BINARY_DIR}/campaignmap-packages
    )
endif()

add_custom_target(
    format
    COMMAND clang-format -i -style=file ${ALL_HEADER_FILES}
    COMMAND clang-format -i -style=file ${ALL_SOURCE_FILES}
    COMMENT "Formatting source files"
)

add_custom_target(
    docs
    COMMAND doxygen ${PROJECT_SOURCE_DIR}/.doxygen.conf
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Generating documentation"
)
