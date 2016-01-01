#ifndef GAME_ENTITY_MANAGER_H
#define GAME_ENTITY_MANAGER_H

#include <QFileInfo>
#include <QJsonDocument>
#include <QMetaObject>

#include "core/Util.h"

namespace warmonger {
namespace core {

class GameEntity;

class EntityManager :
    public QObject
{
    Q_OBJECT

public:
    static EntityManager *getInstance();

    template<typename T>
    T * getEntity(const QString &entityName)
    {
        T *entity = this->findChild<T*>(entityName, Qt::FindDirectChildrenOnly);
        if (entity == nullptr)
            return this->loadEntity<T>(entityName);
        else
            return entity;
    }

    template<typename T>
    T * loadEntity(const QString &entityName)
    {
        QString prefixedFileName = this->getPrefixedFileName(
            &T::staticMetaObject,
            entityName,
            T::fileExtension
        );
        QFileInfo fileInfo(prefixedFileName);
        if (!fileInfo.exists())
            throw IOError("File " + prefixedFileName + " not found");

        T *entity = new T(this);
        this->loadEntityFromFile(entity, fileInfo.absoluteFilePath());

        return entity;
    }

    template<typename T>
    T * loadEntityAs(const QString &entityPath)
    {
        T *entity = new T(this);
        this->loadEntityFromFile(entity, entityPath);

        return entity;
    }

    void saveEntity(const GameEntity *entity) const;

    void saveEntityAs(const GameEntity *entity, const QString &path) const;

    QString getEntityPath(const GameEntity * const entity) const;

private:
    EntityManager();
    ~EntityManager();

    QString getPrefixedFileName(
        const QMetaObject *entityMetaObject,
        const QString &entityName,
        const QString &fileExtension
    ) const;

    void loadEntityFromFile(GameEntity *entity, const QString &path);
    void saveEntityToFile(const GameEntity *entity, const QString &path) const;

    static EntityManager *instance;
    QMap<const GameEntity *, QString> entityPaths;
};

} // namespace warmonger
} // namespace core

#endif // GAME_ENTITY_MANAGER_H
