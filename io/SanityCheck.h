/** \file
 * Sanity-check functions.
 */

#ifndef W_WWRAPPER_SANITY_CHECK_H
#define W_WWRAPPER_SANITY_CHECK_H

class QString;

namespace warmonger {

namespace core {
class World;
} // namespace core

namespace io {

/**
 * Is the world sane?
 *
 * A "sane" world can be loaded without exceptions and it has at least one of
 * every type.
 *
 * \param path the path to the world
 *
 * \return is the world sane
 */
bool isWorldSane(const QString& path);

/**
 * Is the campaign-map sane?
 *
 * A "sane" campaign-map can be loaded without exceptions.
 *
 * \param path the path to the campaign-map
 * \param world a *sane* world
 *
 * \return is the campaign-map sane
 *
 * \see isWorldSane
 */
bool isCampaignMapSane(const QString& path, core::World* world);

} // namespace io
} // namespace warmonger

#endif // W_WWRAPPER_SANITY_CHECK_H
