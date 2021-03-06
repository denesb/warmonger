/** \file
 * WorldRules interface.
 *
 * \copyright (C) 2015-2018 Botond Dénes
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef W_CORE_WORLD_RULES_H
#define W_CORE_WORLD_RULES_H

#include <memory>

#include "core/IntermediateRepresentation.h"

namespace warmonger {
namespace core {

class Map;
class World;
class Faction;

/**
 * World-rules interface.
 *
 * World-rules provide a bridge between the game rules loaded from the world
 * and the game-engine.
 * All calls into the game rules go through this interface.
 */
class WorldRules
{
    Q_GADGET
public:
    /**
     * The types of rules recognised by warmonger.
     */
    enum class Type
    {
        Lua
    };
    Q_ENUM(Type);

    virtual ~WorldRules() = default;

    /**
     * Get the world.
     *
     * \returns the world
     */
    virtual World* getWorld() = 0;

    /**
     * Loads the rules from the path.
     *
     * Parses the rules and calls the world init hook.
     * After this function returns the rules are ready to use.
     *
     * \param basePath the path to the rules directory
     * \param mainRulesFile the name of the rule's main file
     */
    virtual void loadRules(const QString& basePath, const QString& mainRulesFile) = 0;

    /**
     * Generate a map.
     *
     * \param seed to seed the pseudo-random generator with, the same seed
     *      should always generate the same map
     * \param size the radius of the map
     * \param players the faction of players
     *
     * \returns the generated map
     */
    virtual std::unique_ptr<core::Map> generateMap(
        int seed, unsigned int size, std::vector<std::unique_ptr<Faction>> players) = 0;

    /**
     * Do any map-related initialization.
     *
     * \param map the map
     */
    virtual void mapInit(Map* map) = 0;
};

/**
 * Convert the rules-type to string.
 *
 * \param the type
 *
 * \returns the string representation
 *
 * \throws utils::ValueError if type is not a valid enum member.
 */
QString rulesTypeToString(WorldRules::Type type);

/**
 * Parse the rules-type from a string.
 *
 * \param the string
 *
 * \returns the rules-type
 *
 * \throws utils::ValueError if the parsing fails
 */
WorldRules::Type rulesTypeFromString(const QString& str);

/**
 * Creates the rules object for the world.
 *
 * This is a factory function that creates the appropriate rules object
 * for the world's rules-type. It creates and initializes the rules
 * object but the rules themselves are not loaded yet.
 * The rules object will be created as a children of the world.
 *
 * \param world the world
 *
 * \returns the created world-rules object, ready to use
 *
 * \throws ValueError if the rules can't be parsed or initialization fails
 */
std::unique_ptr<WorldRules> createWorldRules(core::World* world);

} // namespace core
} // namespace warmonger

#endif // W_CORE_WORLD_RULES_H
