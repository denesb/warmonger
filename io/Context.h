#ifndef IO_CONTEXT_H
#define IO_CONTEXT_H

#include <functional>

#include <QObject>
#include <QMap>

#include "log/LogStream.h"

namespace warmonger {
namespace io {

/**
 * A context for object unserializations.
 * It can be used to resolve references while unserializing game
 * hierarcy objects. Only usable for QObject-derived classes. Objects
 * are stored by type and objectName. The context does not take
 * ownership of the stored objects.
 */
class Context
{
public:
    Context();

    Context(
        std::function<void (const QString &, const QString &, Context &)> injectFn
    );

    /**
     * Add object to the context.
     */
    void add(QObject *object);

    /**
     * Retrieve the object with objectName `objectName` and type `T`
     * from the context. If the object is not found, nullptr is
     * returned. If there was an `injectFn` set it will be used
     * to inject new objects into the context. This is useful when
     * some dependencies of an object are not now at the point when the
     * context is created. Retriveing an object from the context does
     * not remove it from the context.
     */
    template <typename T>
    T get(const QString &objectName)
    {
        typedef typename std::remove_pointer<T>::type Class;
        const QMetaObject metaObject = Class::staticMetaObject;
        const QString className = metaObject.className();

        wDebug("io.Context") << "Looking up object "
            << className << " with objectName " << objectName;

        QObject *object = this->getObject(className, objectName);
        if (object == nullptr && this->injectFn)
        {
            this->injectFn(className, objectName, *this);
            object = this->getObject(className, objectName);
        }

        return qobject_cast<T>(object);
    }

private:
    QObject * getObject(const QString &className, const QString &objectName) const;

    std::function<void (const QString &, const QString &, Context &)> injectFn;
    QMap<QString, QMap<QString, QObject *>> objectsByType;
};

} // namespace io
} // namespace warmonger

#endif // IO_CONTEXT_H
