#ifndef CORE_LOADER_H
#define CORE_LOADER_H

#include <QDir>
#include <QList>
#include <QMap>
#include <QStringList>
#include <QString>
#include <QObject>

#include "core/JsonUtil.hpp"

namespace warmonger {
namespace core {

template <typename T>
class Loader
{
public:
    Loader(QObject *owner) :
        owner(owner),
        searchPath(),
        pathMap(),
        list()
    {}

    ~Loader()
    {}

    QStringList getSearchPath() const
    {
        return this->searchPath;
    }

    void setSearchPath(const QStringList &searchPath)
    {
        if (this->searchPath != searchPath)
        {
            this->searchPath = searchPath;

            this->scanSearchPath();
            this->loadList();
        }
    }

    QList<T *> getList() const
    {
        return this->list;
    }

    T * get(const QString &name)
    {
        T * foundItem{nullptr};
        for (T *item : this->list)
        {
            if (item->objectName() == name)
            {
                foundItem = item;
            }
        }

        return foundItem;
    }

private:
    void scanSearchPath()
    {
        this->pathMap.clear();

        for (const QString scanPath : this->searchPath)
        {
            QDir scanDir(scanPath);
            scanDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
            scanDir.setSorting(QDir::Name);

            const QString scanCanonicalPath = scanDir.canonicalPath();

            for(const QString dir : scanDir.entryList())
            {
                const QString path = scanCanonicalPath + "/" + dir;
                if (QFile::exists(path + "/" + T::DefinitionFile))
                {
                    this->pathMap[dir] = path;
                }
            }
        }
    }

    void loadList()
    {
        for (T *item : this->list)
        {
            item->deleteLater();
        }
        this->list.clear();

        QMap<QString, QString>::ConstIterator it;
        for (it = this->pathMap.constBegin(); it != this->pathMap.constEnd(); it++)
        {
            this->list.append(newFromJsonFile<T>(it.value() + "/" + T::DefinitionFile, owner));
        }
    }

    QObject *owner;
    QStringList searchPath;
    QMap<QString, QString> pathMap;
    QList<T *> list;
};

} // namespace core
} // namespace warmonger

#endif // CORE_LOADER_H
