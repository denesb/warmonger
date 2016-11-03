#ifndef W_UTILS_H
#define W_UTILS_H

#include <QObject>
#include <QString>

namespace warmonger {
namespace utils {

struct QObjectFinder
{
    QObjectFinder(const QString& name)
        : name(name)
    {
    }

    bool operator()(const QObject* obj)
    {
        return obj->objectName() == this->name;
    }

    QString name;
};

/**
 * RAII class the block signals in the constructor and unblocks in the
 * destructor.
 */
class SignalBlockGuard
{
public:
    SignalBlockGuard(QObject* const obj)
        : obj(obj)
    {
        obj->blockSignals(true);
    }

    ~SignalBlockGuard()
    {
        obj->blockSignals(false);
    }

private:
    QObject* const obj;
};

template <typename Head, typename Tail>
Head makePath(const Head& head, const Tail& tail)
{
    return head + "/" + tail;
}

template <typename Head, typename... Component>
Head makePath(const Head& head, const Component&... components)
{
    return head + "/" + makePath(components...);
}

QString makeFileName(const QString& fileName, const QString& extension);

struct DelayedQObjectDeleter
{
    constexpr DelayedQObjectDeleter() = default;

    void operator()(QObject* object) const
    {
        object->deleteLater();
    }
};

} // namespace utils
} // namespace warmonger

#endif // W_UTILS_H
