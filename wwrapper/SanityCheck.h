#ifndef W_WWRAPPER_SANITY_CHECK_H
#define W_WWRAPPER_SANITY_CHECK_H

class QString;

namespace warmonger {
namespace wwrapper {

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

} // namespace wwrapper
} // namespace warmonger

#endif // W_WWRAPPER_SANITY_CHECK_H
