#ifndef CORE_UTIL_H
#define CORE_UTIL_H

#include <QString>
#include <QPoint>
#include <QObject>
#include <QVariant>

#include "core/Exception.h"
#include "log/LogStream.h"

namespace warmonger {
namespace core {

template<typename T>
QList<const T *> listConstClone(const QList<T *> &list)
{
    QList<const T *> constList;

    for (const T *i : list)
    {
        constList << i;
    }

    return std::move(constList);
}

template<typename T>
QVariantList toQVariantList(const QList<T *> &list)
{
    QVariantList vlist;

    for (T *i : list)
    {
        vlist << QVariant::fromValue<QObject *>(i);
    }

    return std::move(vlist);
}

template<typename T>
QObjectList toQObjectList(const QList<T *> &list)
{
    QObjectList olist;

    for (T *i : list)
    {
        olist << i;
    }

    return std::move(olist);
}

template<typename T>
T* resolveReference(const QString &objectName, QObject *parent, const QString &moduleName)
{
    T *obj = parent->findChild<T *>(objectName);
    if (obj == nullptr)
    {
        Exception e(Exception::UnresolvedReference, {"T", objectName});
        wError(moduleName) << e.getMessage();
        throw e;
    }
    return obj;
}

} // namespace core
} // namespace warmonger

#endif // CORE_UTIL_H
