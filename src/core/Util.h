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

/**
 * Convert a QList<T> to a QVariantList.
 *
 * T must be a convertable to QVariant!
 */
template<typename T>
QVariantList toQVariantList(const QList<T> &list)
{
    QVariantList vlist;

    for (T i : list)
    {
        vlist << QVariant::fromValue<T>(i);
    }

    return std::move(vlist);
}

/**
 * Convert a QList<T> to a QVariant.
 *
 * T must be convertable to QVariant!
 */
template <typename T>
QVariant listToQVariant(const QList<T> list)
{
    return QVariant::fromValue<QList<QVariant>>(toQVariantList<T>(list));
}

/**
 * Convert a QVariantList to a QList<T>.
 *
 * Throws QVariantTypeError when a list item cannot be converted to T!
 */
template<typename T>
QList<T> fromQVariantList(QVariantList vlist)
{
    QList<T> list;

    for (QVariant v : vlist)
    {
        if (v.canConvert<T>())
            throw QVariantTypeError();

        list << v.value<T>();
    }

    return std::move(list);
}

/**
 * Convert a Qt-style mapping-type to QVariantMap.
 *
 * Qt-style means that the iterator has a key() and value() member
 * returning the key and value respectively.
 * For the conversion a `convertKey` and a `convertValue` function must
 * be supplied, where `convertKey` converts from K to QString and
 * `convertValue converts from `V` to QVariant.
 */
template<typename Iterator, typename ConvertKeyFunc, typename ConvertValueFunc>
QVariantMap toQVariantMap(
    Iterator first,
    Iterator last,
    ConvertKeyFunc convertKey,
    ConvertValueFunc convertValue
)
{
    QVariantMap vmap;

    for (Iterator it = first; it != last; it++)
    {
        QString key = convertKey(it.key());
        QVariant value = convertValue(it.value());
        vmap[key] = value;
    }

    return std::move(vmap);
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
