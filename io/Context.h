#ifndef IO_CONTEXT_H
#define IO_CONTEXT_H

#include <typeinfo>
#include <typeindex>

#include <QObject>
#include <QMap>

namespace warmonger {
namespace io {

/**
 * A context for object unserializations.
 * It can be used to resolve references while unserializing game
 * hierarcy objects. Only usable for QObject-derived classes. Objects
 * are stored by type and name. The context does not take ownership
 * of the stored objects.
 */
class Context
{
public:
    /**
     * Add object to the context.
     */
    template <typename T>
    void add(T object)
    {
        std::type_index tindex(typeid(typename std::remove_pointer<T>::type));

        QMap<QString, QObject *> &typeObjs = this->objects[tindex];
        typeObjs[object->objectName()] = object;
    }

    /**
     * Retrieve the object with name `name` and type `T` from the
     * context. If the object is not found, nullptr is returned.
     * Retriveing an object from the context does not remove it from
     * the context.
     */
    template <typename T>
    T get(const QString &name)
    {
        std::type_index tindex(typeid(typename std::remove_pointer<T>::type));

        QMap<QString, QObject *> typeObjs = this->objects[tindex];
        QObject *o = typeObjs[name];

        return qobject_cast<T>(o);
    }

private:
    QMap<std::type_index, QMap<QString, QObject *>> objects;
};

} // namespace core
} // namespace warmonger

#endif // IO_CONTEXT_H
