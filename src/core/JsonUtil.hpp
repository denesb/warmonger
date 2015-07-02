#ifndef CORE_JSON_UTIL_HPP
#define CORE_JSON_UTIL_HPP

#include <QObject>
#include <QString>
#include <QList>
#include <QMap>
#include <QFile>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>

namespace core {

template<typename T>
T * newFromJson(const QJsonObject &obj, QObject *parent = nullptr)
{
    T *instance = new T(parent);
    instance->fromJson(obj);
    return instance;
}

template<typename T>
QList<T *> newListFromJson(const QJsonArray &array, QObject *parent = nullptr)
{
    QList<T *> list;

    for (const QJsonValue v : array)
    {
        T *instance = new T(parent);
        instance->fromJson(v.toObject());
        list.append(instance);
    }

    return std::move(list);
}

template<typename T>
QJsonArray listToJson(const QList<T *> &list)
{
    QJsonArray array;

    for (const T *instance : list)
    {
        array.append(instance->toJson());
    }

    return std::move(array);
}

template<typename T>
QList<T *> referenceListFromJson(const QJsonArray &array, QObject *owner)
{
    QList<T *> list;
    QObject *parent = owner->parent();

    for (const QJsonValue v : array)
    {
        const QString name = v.toString();
        T * instance = parent->findChild<T *>(name);
        list.append(instance);
    }

    return std::move(list);
}

template<typename T>
QJsonArray referenceListToJson(const QList<T *> &list)
{
    QJsonArray array;

    for (const T * instance : list)
    {
        array.append(instance->objectName());
    }

    return std::move(array);
}

template<typename T>
QMap<T *, int> objectValueMapFromJson(const QJsonObject &obj, const QObject * const owner)
{
    QMap<T *, int> map;
    QObject *parent = owner->parent();

    QJsonObject::const_iterator it;
    for(it = obj.constBegin(); it != obj.constEnd(); it++)
    {
        const QString name = it.key();
        T *instance = parent->findChild<T *>(name);

        map[instance] = it.value().toInt();
    }

    return std::move(map);
}

template<typename T>
QJsonObject objectValueMapToJson(const QMap<T *, int> &map)
{
    QJsonObject obj;

    typename QMap<T *, int>::const_iterator it;
    for(it = map.constBegin(); it != map.constEnd(); it++)
    {
        T *instance = it.key();
        obj[instance->objectName()] = it.value();
    }
    
    return std::move(obj);
}

template<typename T>
T * newFromJsonFile(const QString &path, QObject *parent = nullptr)
{
    QFile jsonFile(path);

    if (!jsonFile.open(QIODevice::ReadOnly))
    {
        //TODO: throw something
    }

    QByteArray jsonData = jsonFile.readAll();

    jsonFile.close();
    
    QJsonParseError parseError; //TODO: do something with this

    QJsonDocument doc(QJsonDocument::fromJson(jsonData, &parseError));
    QJsonObject obj = doc.object();

    return newFromJson<T>(obj, parent);
}

}; // namespace core

#endif // CORE_JSON_UTIL_HPP
