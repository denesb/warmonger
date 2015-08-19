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
        pathMap()
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
        }
    }

    T * load(const QString &name) const
    {
        const QString path(this->pathMap[name] + "/" + T::DefinitionFile);

        QJsonDocument doc = loadJsonDocument(path);

        QJsonObject obj = doc.object();

        T *instance = new T(this->owner);
        instance->setPath(this->pathMap[name]);
        instance->fromJson(obj);
        return instance;
    }

    QList<T *> loadList() const
    {
        QList<T *> list;

        QMap<QString, QString>::ConstIterator it;
        for (it = this->pathMap.constBegin(); it != this->pathMap.constEnd(); it++)
        {
            list.append(this->load(it.key()));
        }

        return std::move(list);
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

    QObject *owner;
    QStringList searchPath;
    QMap<QString, QString> pathMap;
};

} // namespace core
} // namespace warmonger

#endif // CORE_LOADER_H
