#ifndef W_UTILS_H
#define W_UTILS_H

#include <QString>

struct QObjectFinder
{
    QObjectFinder(const QString &name) : name(name)
    {}

    bool operator()(const QObject *obj)
    {
        return obj->objectName() == this->name;
    }

    QString name;
};

#endif // W_UTILS_H
