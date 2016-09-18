#include "io/Context.h"

namespace warmonger {
namespace io {

Context::Context()
    : injectFn()
    , objectsByType()
{
}

Context::Context(std::function<void(const QString&, const QString&, Context&)> injectFn)
    : injectFn(injectFn)
    , objectsByType()
{
}

void Context::add(QObject* object)
{
    const QMetaObject* metaObject = object->metaObject();
    const QString className = metaObject->className();

    wDebug << "Added `" << className << "' object `" << object << "'";

    QMap<QString, QObject*>& objects = this->objectsByType[className];
    objects[object->objectName()] = object;
}

QObject* Context::getObject(const QString& className, const QString& objectName) const
{
    const QMap<QString, QObject*> objects = this->objectsByType[className];

    QObject* object{nullptr};
    if (objects.contains(objectName))
    {
        object = objects[objectName];
    }
    else
    {
        wWarning << "Object `" << className << "' with objectName `" << objectName << "' not found";
    }

    return object;
}

} // namespace io
} // namespace warmonger
