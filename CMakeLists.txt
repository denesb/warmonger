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

cmake_minimum_required(VERSION 3.1.3)

project(Warmonger)

option(BUILD_TOOLS "Build development tools" OFF)
option(BUILD_TESTS "Build unit test suite" OFF)

set(CMAKE_CXX_STANDARD 14)

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
    src/utils/Settings.cpp
    src/utils/ToString.cpp
    src/utils/Utils.cpp
)

set(
    CORE_SRC_FILES
    src/core/Banner.cpp
    src/core/BuiltInComponentTypes.cpp
    src/core/Civilization.cpp
    src/core/Component.cpp
    src/core/ComponentType.cpp
    src/core/Entity.cpp
    src/core/Faction.cpp
    src/core/Field.cpp
    src/core/Hexagon.cpp
    src/core/LuaWorldRules.cpp
    src/core/Map.cpp
    src/core/MapNode.cpp
    src/core/MapNodeNeighbours.cpp
    src/core/Utils.cpp
    src/core/WObject.cpp
    src/core/World.cpp
    src/core/WorldComponentType.cpp
    src/core/WorldRules.cpp
)

set(
    IO_SRC_FILES
    src/io/File.cpp
    src/io/JsonUnserializer.cpp
    src/io/MapJsonSerializer.cpp
    src/io/MapJsonUnserializer.cpp
    src/io/Reference.cpp
    src/io/WorldJsonSerializer.cpp
    src/io/WorldJsonUnserializer.cpp
)

set(
    UI_SRC_FILES
    src/ui/Banner.cpp
    src/ui/BasicMap.cpp
    src/ui/BasicMiniMap.cpp
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
)

set(
    TEST_SRC_FILES
    src/test/Util.cpp
    src/test/WObject.cpp
    src/test/core/Map.cpp
    src/test/core/MapNodeNeighbours.cpp
    src/test/core/WObject.cpp
    src/test/io/File.cpp
    src/test/io/MapJsonSerializer.cpp
    src/test/io/MapJsonUnserializer.cpp
    src/test/io/Reference.cpp
    src/test/io/WorldJsonSerializer.cpp
    src/test/io/WorldJsonUnserializer.cpp
    src/test/test_warmonger.cpp
    src/test/ui/MapEditor.cpp
    src/test/ui/MapUtil.cpp
    src/test/ui/MapWindow.cpp
    src/test/ui/WorldSurface.cpp
)

file(GLOB_RECURSE ALL_HEADER_FILES ${PROJECT_SOURCE_DIR}/src/*.h)
file(GLOB_RECURSE ALL_SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Libraries
find_package(Qt5Core REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(KF5Archive REQUIRED)
find_package(Threads REQUIRED)
find_package(Lua REQUIRED)

include(warmonger_automoc.cmake)

include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/thirdparty
    ${PROJECT_BINARY_DIR}
    ${LUA_INCLUDE_DIR}
)

set(
    COMMON_EXTERNAL_LIBS
    Qt5::Quick
    Qt5::Core
    ${LUA_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
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
    src/wmapeditor/Context.cpp
    src/wmapeditor/wmapeditor.cpp
    src/wmapeditor/wmapeditor.qrc
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
    src/warmonger/Context.cpp
    src/warmonger/warmonger.cpp
    src/warmonger/warmonger.qrc
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
        src/tools/wcheck_world.cpp
        src/tools/Utils.cpp
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
        wcheck_map
        src/tools/wcheck_map.cpp
        src/tools/Utils.cpp
    )

    target_link_libraries(
        wcheck_map
        ui
        io
        core
        utils
        ${COMMON_EXTERNAL_LIBS}
    )

    add_executable(
        wcheck_worldsurface
        src/tools/Utils.cpp
        src/tools/wcheck_worldsurface.cpp
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
        src/tools/wcreate_default_settings.cpp
    )

    target_link_libraries(
        wcreate_default_settings
        utils
        ${COMMON_EXTERNAL_LIBS}
    )

    add_executable(
        wtest_rules_bindings
        src/tools/wtest_rules_bindings.cpp
    )

    target_link_libraries(
        wtest_rules_bindings
        core
        utils
        ${COMMON_EXTERNAL_LIBS}
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
        ui
        io
        core
        utils
        ${COMMON_EXTERNAL_LIBS}
    )

    set(TEST_DATA "${CMAKE_SOURCE_DIR}/src/test/data")
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
        ${TEST_DATA}/map-packages
        ${CMAKE_BINARY_DIR}/map-packages
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
