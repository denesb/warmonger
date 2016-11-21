/** \file
 * IO Context.
 */

#ifndef IO_CONTEXT_H
#define IO_CONTEXT_H

#include <functional>

#include <QMap>
#include <QObject>

#include "utils/Logging.h"

namespace warmonger {

namespace core {
class World;
} // namespace core

namespace io {

/**
 * A context for object unserializations.
 *
 * It is used to resolve references while unserializing game hierarcy objects.
 * Only usable for QObject-derived classes. Objects are stored by type and
 * objectName. The context does not take ownership of the stored objects.
 */
class Context
{
public:
    /**
     * Construct an empty Context object.
     */
    Context();

    /**
     * Construct an empty Context object with an injector functor.
     *
     * The injector function is called every time the context fails to look up
     * a requested name. The injector is thus given a chance to inject the
     * the object. This can be useful to inject dependencies on-demand when it
     * is not possible to add all possible object to the context (because they
     * might have conflicting names).
     * The inject functor is called with the class-name, the object-name and
     * the context object itself.
     *
     * \param injectFn the inject functor
     */
    Context(std::function<void(const QString&, const QString&, Context&)> injectFn);

    /**
     * Add object to the context.
     *
     * \param object the object
     */
    void add(QObject* object);

    /**
     * Retrieve the object with objectName and type T from the context.
     *
     * If the object is not found, nullptr is returned. If there was an inject
     * functor set it will be used to inject new objects into the context before
     * giving up. Retriveing an object from the context does not remove it from
     * the context.
     *
     * \param objectName the name of the object
     *
     * \see Context::Context(std::function<void(const QString&, const QString&, Context&)>)
     */
    template <typename T>
    T get(const QString& objectName)
    {
        typedef typename std::remove_pointer<T>::type Class;
        const QMetaObject metaObject = Class::staticMetaObject;
        const QString className = metaObject.className();

        wDebug << "Looking up object `" << className << "' with objectName `" << objectName << "'";

        QObject* object = this->getObject(className, objectName);
        if (object == nullptr && this->injectFn)
        {
            this->injectFn(className, objectName, *this);
            object = this->getObject(className, objectName);
        }

        if (object == nullptr)
        {
            wWarning << "Failed to find object `" << className << "' with objectName `" << objectName << "'";
        }

        return qobject_cast<T>(object);
    }

private:
    QObject* getObject(const QString& className, const QString& objectName) const;

    std::function<void(const QString&, const QString&, Context&)> injectFn;
    QMap<QString, QMap<QString, QObject*>> objectsByType;
};

/**
 * Add an entire world to the context.
 *
 * Convenience function to add an entire world and all that's in it to the
 * context.
 *
 * \param ctx the context
 * \param world the world
 */
void addWorldToContext(Context& ctx, core::World* world);

} // namespace io
} // namespace warmonger

#endif // IO_CONTEXT_H
