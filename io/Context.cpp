#include "io/Context.h"

using namespace warmonger;
using namespace warmonger::io;

Context::Context() :
    injectFn(),
    objectsByType()
{
}

Context::Context(
    std::function<void (const QString &, const QString &, Context &)> injectFn
) :
    injectFn(injectFn),
    objectsByType()
{
}

void Context::add(QObject *object)
{
    const QMetaObject *metaObject = object->metaObject();
    const QString className = metaObject->className();

    wDebug("io.Context") << "Added " << className << " object " << object;

    QMap<QString, QObject *> &objects = this->objectsByType[className];
    objects[object->objectName()] = object;
}

QObject * Context::getObject(
    const QString &className,
    const QString &objectName
) const
{
    const QMap<QString, QObject *> objects = this->objectsByType[className];

    QObject *object{nullptr};
    if (objects.contains(objectName))
    {
        object = objects[objectName];
    }

    return object;
}
