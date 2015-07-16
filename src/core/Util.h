#ifndef CORE_UTIL_H
#define CORE_UTIL_H

#include <QString>
#include <QPoint>

namespace warmonger {
namespace core {

template <typename T>
QList<const T *> listConstClone(const QList<T *> &list)
{
    QList<const T *> constList;

    for (const T *i : list)
    {
        constList << i;
    }

    return std::move(constList);
}

} // namespace core
} // namespace warmonger

#endif // CORE_UTIL_H
