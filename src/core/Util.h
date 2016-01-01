#ifndef CORE_UTIL_H
#define CORE_UTIL_H

#include <QString>
#include <QPoint>
#include <QObject>
#include <QVariant>

#include "core/GameEntity.h"
#include "core/Exception.h"
#include "log/LogStream.h"

namespace warmonger {
namespace core {

static const QString _loggerName{"core.Util"};

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
QList<T *> fromQVariantList(QVariantList vlist)
{
    QList<T *> list;

    for (QVariant v : vlist)
    {
        if (v.canConvert<T *>())
            throw QVariantTypeError();

        list << v.value<T *>();
    }

    return std::move(list);
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
T* resolveReference(const QString &objectName, const QObject * const parent)
{
    T *obj = parent->findChild<T *>(objectName);
    if (obj == nullptr)
    {
        throw UnresolvedReferenceError(objectName);
    }

    return obj;
}

QString getClassName(const QString &fullClassName);

} // namespace core
} // namespace warmonger

#endif // CORE_UTIL_H
