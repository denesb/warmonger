/** \file
 * Campaign-map context.
 */

#ifndef W_WMAPEDITOR_CONTEXT_H
#define W_WMAPEDITOR_CONTEXT_H

#include "ui/Context.h"

namespace warmonger {
namespace wmapeditor {

/**
 * Provides a campaign-map editor specific context for the QML code.
 *
 * Context is main bridge between C++ and QML/JS code. It is accessable as a
 * globally available object. It provides the current world, world-surface,
 * campaign-map the list of available worlds, world-surfaces, campaign-maps,
 * palette-colors and others.
 * It also provides functions to access core functionality.
 * This context provides functions specific to the camaign-map editor.
 */
class Context : public ui::Context
{
    Q_OBJECT

public:
    using ui::Context::Context;

    /**
     * Error
     */
    enum class ErrorCategory
    {
        IOError,
        ContentError,
        InternalError,
        UnknownError
    };
    Q_ENUM(ErrorCategory)

    /**
     * Create a new map.
     *
     * The map will be set as the current map and the world as the
     * current world.
     */
    Q_INVOKABLE void create(warmonger::core::World* world);

    /**
     * Save the current campaign-map.
     *
     * The map will be saved to the current path for the campaign-map.
     * This will be the path used for the last save operation. If there
     * was no previous save, the map will be saved to the default map
     * location for the world: {world-path}/maps/{object-name}.wmd.
     * Where world-path is the path to the directory where the current world
     * is and object-name is the object-name of the map.
     * If the save fails retrieve the error with Context::getLastError().
     *
     * \return true if the save succeded, false otherwise
     */
    Q_INVOKABLE bool save();

    /**
     * Save the current campaign-map as path.
     *
     * The map will be saved to path. The path has to be writable.
     * If the save fails retrieve the error with Context::getLastError().
     *
     * \return true if the save succeded, false otherwise
     *
     * \see Context::save()
     */
    Q_INVOKABLE bool saveAs(const QString& path);

    /**
     * Load the campaign-map from path.
     *
     * If the load is successfull campaign-map will be the current
     * campaign-map and it's world the current world.
     * If the load fails retrieve the error with Context::getLastError().
     *
     * \return true if the load succeded, false otherwise
     */
    Q_INVOKABLE bool load(const QString& path);

    /**
     * Get the path used for the last save operation.
     *
     * Retrieve the path used in the last call of Context::save() or
     * Context::saveAs() call.
     *
     * \return the last path
     */
    Q_INVOKABLE QString getPath() const
    {
        return this->lastPath;
    }

    /**
     * Get the error category for the last failed operation.
     *
     * Retrieve the error category for the last failed Context::save(),
     * Context::saveAs() or Context::load() call.
     *
     * \return the error category
     */
    Q_INVOKABLE ErrorCategory getLastErrorCategory() const
    {
        return this->lastErrorCategory;
    }
    /**
     * Get the error message for the last failed operation.
     *
     * Retrieve the error message for the last failed Context::save(),
     * Context::saveAs() or Context::load() call.
     *
     * \return the human-readable error message
     */
    Q_INVOKABLE QString getLastErrorMessage() const
    {
        return this->lastErrorMessage;
    }

private:
    ErrorCategory lastErrorCategory;
    QString lastErrorMessage;
    QString lastPath;
};

} // namespace wmapeditor
} // namespace warmonger

#endif // W_WMAPEDITOR_CONTEXT_H
