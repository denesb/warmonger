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

template <typename T>
T * resolveReference(QObject *node, const QString &objectName)
{
    QObject *parent = node->parent();
    T *object{nullptr};

    if (parent != nullptr)
    {
        object = parent->findChild<T *>(objectName);

        if (object == nullptr)
        {
            object = resolveReference(parent, objectName);
        }
    }

    return object;
}

}; // namespace core
}; // namespace warmonger

#endif // CORE_UTIL_H
