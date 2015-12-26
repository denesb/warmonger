#include "core/EntityManager.h"
#include "core/Exception.h"
#include "core/JsonUtil.h"
#include "log/LogStream.h"

static const QString category("core");

using namespace warmonger;
using namespace warmonger::core;

EntityManager * EntityManager::instance{nullptr};

EntityManager * EntityManager::getInstance()
{
    if (EntityManager::instance == nullptr)
        EntityManager::instance = new EntityManager();

    return EntityManager::instance;
}

void EntityManager::saveEntity(const GameEntity *entity) const
{
    this->saveEntityToFile(entity, this->entityPaths[entity]);
}

void EntityManager::saveEntityAs(
    const GameEntity *entity,
    const QString &path
) const
{
    this->saveEntityToFile(entity, path);
}

EntityManager::EntityManager() :
    QObject(nullptr),
    entityPaths()
{
}

EntityManager::~EntityManager()
{
}

QString EntityManager::getPrefixedFileName(
    const QMetaObject *entityMetaObject,
    const QString &entityName,
    const QString &fileExtension
) const
{
    static const QString pfnTemplate{"%1:%2.%3"};

    QString className = getClassName(entityMetaObject->className());

    return pfnTemplate.arg(
        className,
        entityName,
        fileExtension
    );
}

void EntityManager::loadEntityFromFile(GameEntity *entity, const QString &path)
{
    this->entityPaths[entity] = path;

    QJsonDocument doc = loadJsonDocument(path);
    entity->fromJson(doc.object());
}

void EntityManager::saveEntityToFile(
    const GameEntity *entity,
    const QString &path
) const
{
    QJsonDocument doc(entity->toJson());
    saveJsonDocument(path, doc);
}


QString EntityManager::getEntityPath(const GameEntity * entity) const
{
    return this->entityPaths[entity];
}
