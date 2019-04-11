#
# Copyright (C) 2015-2018 Botond Dénes
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

cmake_minimum_required(VERSION 3.8.2)

project(Warmonger)

option(BUILD_TOOLS "Build development tools" OFF)
option(BUILD_TESTS "Build unit test suite" OFF)
option(USE_ASAN "Use ASAN" OFF)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

add_compile_options(
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

# Source files

set(
    UTILS_SRC_FILES
    src/utils/Logging.cpp
    src/utils/Lua.cpp
    src/utils/Settings.cpp
    src/utils/ToString.cpp
    src/utils/Utils.cpp
    src/utils/PathBuilder.cpp
)

set(
    CORE_SRC_FILES
    src/core/Faction.cpp
    src/core/Hexagon.cpp
    src/core/IntermediateRepresentation.cpp
    src/core/LuaWorldRules.cpp
    src/core/Map.cpp
    src/core/MapNode.cpp
    src/core/Settlement.cpp
    src/core/WObject.cpp
    src/core/World.cpp
    src/core/WorldRules.cpp
)

set(
    IO_SRC_FILES
    src/io/File.cpp
    src/io/JsonSerializer.cpp
)

set(
    UI_SRC_FILES
    src/ui/Banner.cpp
    src/ui/BasicMap.cpp
    src/ui/BasicMiniMap.cpp
    src/ui/LuaWorldSurfaceRules.cpp
    src/ui/MapEditor.cpp
    src/ui/MapUtil.cpp
    src/ui/MapView.cpp
    src/ui/MapWatcher.cpp
    src/ui/MapWindow.cpp
    src/ui/MiniMap.cpp
    src/ui/Palette.cpp
    src/ui/Render.cpp
    src/ui/SearchPaths.cpp
    src/ui/UI.cpp
    src/ui/WorldSurface.cpp
    src/ui/WorldSurfaceRules.cpp
)

set(
    TEST_SRC_FILES
    src/test/WObject.cpp
    src/test/core/Map.cpp
    src/test/core/MapNodeNeighbours.cpp
    src/test/core/WObject.cpp
    src/test/io/Serializer.cpp
    src/test/test_warmonger.cpp
    src/test/ui/MapEditor.cpp
    src/test/ui/MapUtil.cpp
    src/test/ui/MapWindow.cpp
)

file(GLOB_RECURSE ALL_HEADER_FILES ${PROJECT_SOURCE_DIR}/src/*.h)
file(GLOB_RECURSE ALL_SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

if (USE_ASAN)
    add_compile_options(-fsanitize=address -fsanitize=leak -fsanitize=undefined)
    set(CMAKE_EXE_LINKER_FLAGS "-lasan -lubsan")
endif()

# Libraries
find_package(Qt5Core REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Threads REQUIRED)
find_package(KF5Archive REQUIRED)
#TODO: create proper imported target for lua
find_package(Lua REQUIRED)
find_package(fmt REQUIRED)

# Why to I have to do this??????
set_target_properties(Qt5::Core PROPERTIES INTERFACE_LINK_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})

include(warmonger_automoc.cmake)

#TODO set this in target_inlude_directories
include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/thirdparty
    ${PROJECT_BINARY_DIR}
    ${LUA_INCLUDE_DIR}
)

# Targets

add_library(utils STATIC ${UTILS_SRC_FILES})
target_link_libraries(
    utils
    PUBLIC Qt5::Core Qt5::Quick fmt::fmt
)
WARMONGER_AUTOMOC(utils)

add_library(core STATIC ${CORE_SRC_FILES})
target_link_libraries(
    core
    PUBLIC Qt5::Core Qt5::Quick utils
    PRIVATE ${LUA_LIBRARIES}
)
WARMONGER_AUTOMOC(core)

add_library(io STATIC ${IO_SRC_FILES})
target_link_libraries(
    io
    PUBLIC Qt5::Core Qt5::Quick core utils
)
WARMONGER_AUTOMOC(io)

add_library(ui STATIC ${UI_SRC_FILES})
target_link_libraries(ui
    PUBLIC Qt5::Core Qt5::Quick io core utils
    PRIVATE KF5::Archive
)
WARMONGER_AUTOMOC(ui)

add_executable(
    wmapeditor
    src/wmapeditor/Context.cpp
    src/wmapeditor/wmapeditor.cpp
    src/wmapeditor/wmapeditor.qrc
)
WARMONGER_AUTOMOC(wmapeditor)

target_link_libraries(
    wmapeditor
    PRIVATE ui io core utils
)

add_executable(
    warmonger
    src/warmonger/Context.cpp
    src/warmonger/warmonger.cpp
    src/warmonger/warmonger.qrc
)
WARMONGER_AUTOMOC(warmonger)

target_link_libraries(
    warmonger
    PRIVATE ui io core utils
)

if(BUILD_TOOLS)
    add_executable(
        wcheck_world
        src/tools/wcheck_world.cpp
        src/tools/Utils.cpp
    )

    target_link_libraries(
        wcheck_world
        PRIVATE ui io core utils
    )

    add_executable(
        wcheck_map
        src/tools/wcheck_map.cpp
        src/tools/Utils.cpp
    )

    target_link_libraries(
        wcheck_map
        PRIVATE ui io core utils
    )

    add_executable(
        wcheck_worldsurface
        src/tools/Utils.cpp
        src/tools/wcheck_worldsurface.cpp
    )

    target_link_libraries(
        wcheck_worldsurface
        PRIVATE ui io core utils
    )

    add_executable(
        wcreate_default_settings
        src/tools/wcreate_default_settings.cpp
    )

    target_link_libraries(
        wcreate_default_settings
        PRIVATE utils
    )

    add_executable(
        wtest_rules_bindings
        src/tools/wtest_rules_bindings.cpp
    )

    target_link_libraries(
        wtest_rules_bindings
        PRIVATE core utils
    )
endif()

if(BUILD_TESTS)
    add_executable(
        test_warmonger
        ${TEST_SRC_FILES}
        src/test/test_warmonger.cpp
    )
    WARMONGER_AUTOMOC(test_warmonger)

    target_link_libraries(
        test_warmonger
        PRIVATE ui io core utils
    )
endif()

add_custom_target(
    format
    COMMAND clang-format -i -style=file ${ALL_HEADER_FILES}
    COMMAND clang-format -i -style=file ${ALL_SOURCE_FILES}
    COMMENT "Formatting source files"
)

configure_file(doxygen.conf.in doxygen.conf @ONLY)
add_custom_target(
    docs
    COMMAND doxygen ${PROJECT_BINARY_DIR}/doxygen.conf
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "Generating documentation"
)
