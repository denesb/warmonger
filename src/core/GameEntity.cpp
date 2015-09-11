#include <QFileInfo>

#include "core/GameEntity.h"
#include "core/JsonUtil.h"

using namespace warmonger::core;

static const QString category("core");

QString getClassName(const QMetaObject *metaObject);

QObject *GameEntity::owner{nullptr};
QMap<QString, GameEntity *> GameEntity::knownEntities{};
QList<GameEntity *> GameEntity::anonymusEntities{};

GameEntity::GameEntity(QObject *parent) :
    GameObject(parent),
    path(),
    fileName(),
    description("")
{
    GameEntity::anonymusEntities << this;
    QObject::connect(
        this,
        &QObject::objectNameChanged,
        this,
        &GameEntity::onObjectNameChanged
    );
}

GameEntity::~GameEntity()
{
    GameEntity::anonymusEntities.removeOne(this);
    GameEntity::knownEntities.remove(this->entityKey());
}

QString GameEntity::getPath() const
{
    return this->path;
}

QString GameEntity::getFileName() const
{
    return this->fileName;
}

QString GameEntity::getDescription() const
{
    return this->description;
}

void GameEntity::setDescription(const QString &description)
{
    if (this->description != description)
    {
        this->description = description;
        emit descriptionChanged();
    }
}

QObject * GameEntity::getOwner()
{
    return GameEntity::owner;
}

void GameEntity::setOwner(QObject *owner)
{
    GameEntity::owner = owner;

    QMap<QString, GameEntity *>::Iterator it;
    QMap<QString, GameEntity *> *knownEntities = &GameEntity::knownEntities;
    for (it = knownEntities->begin(); it != knownEntities->end(); it++)
    {
        it.value()->setParent(owner);
    }

    for (GameEntity *entity : GameEntity::anonymusEntities)
    {
        entity->setParent(owner);
    }
}

GameEntity * GameEntity::get(
    const QString &objectName,
    const QMetaObject *metaObject
)
{
    GameEntity *entity = nullptr;
    const QString key = GameEntity::entityKey(objectName, metaObject);
    if (GameEntity::knownEntities.contains(key)) {
        entity = GameEntity::knownEntities[key];
    }
    else
    {
        entity = GameEntity::load(objectName, metaObject);
    }

    return entity;
}

GameEntity * GameEntity::load(
    const QString &objectName,
    const QMetaObject *metaObject
)
{
    QObject *object = metaObject->newInstance(
        Q_ARG(QObject *, nullptr)
    );

    GameEntity *entity = qobject_cast<GameEntity *>(object);

    entity->setObjectName(objectName);
    entity->load();
    return entity;
}

void GameEntity::load()
{
    QString prefixedFileName = this->getPrefixedFileName();
    QFileInfo fileInfo(prefixedFileName);
    if (!fileInfo.exists())
    {
        wError(category) << "File " << fileName << " not found";
        throw Exception(Exception::FileIO, "File not found");
    }

    this->setPath(fileInfo.absolutePath());
    this->setFileName(fileInfo.fileName());

    this->loadFromFile(fileInfo.absoluteFilePath());
}

void GameEntity::loadAs(const QString &path)
{
    QFileInfo fileInfo(path);
    this->setPath(fileInfo.absolutePath());
    this->setFileName(fileInfo.fileName());

    this->loadFromFile(fileInfo.absoluteFilePath());
}

void GameEntity::save() const
{
    const QString path(this->path + "/" + this->fileName);
    this->saveToFile(path);
}

void GameEntity::saveAs(const QString &path) const
{
    this->saveToFile(path);
}

void GameEntity::setPath(const QString &path)
{
    if (!this->path.isNull())
    {
        wError(category) << "Entity " << this->objectName() << " already loaded";
        throw Exception(Exception::EntityAlreadyLoaded);
    }
    this->path = path;

    emit this->pathChanged();
}

void GameEntity::setFileName(const QString &fileName)
{
    if (!this->fileName.isNull())
    {
        wError(category) << "Entity " << this->objectName() << " already loaded";
        throw Exception(Exception::EntityAlreadyLoaded);
    }
    this->fileName = fileName;

    emit this->fileNameChanged();
}

QString GameEntity::getPrefixedFileName() const
{
    static const QString pfnTemplate{"%1:%2.%3"};

    const QMetaObject *metaObject = this->metaObject();
    QString className = getClassName(metaObject);

    return pfnTemplate.arg(
        className,
        this->objectName(),
        this->getFileExtension()
    );
}

void GameEntity::loadFromFile(const QString &path)
{
    QJsonDocument doc = loadJsonDocument(path);
    this->fromJson(doc.object());

    emit this->loaded();
}

void GameEntity::saveToFile(const QString &path) const
{
    QJsonDocument doc(this->toJson());
    saveJsonDocument(path, doc);

    emit this->saved();
}

void GameEntity::dataFromJson(const QJsonObject &obj)
{
    this->description = obj["description"].toString();
}

void GameEntity::dataToJson(QJsonObject &obj) const
{
    obj["description"] = this->description;
}

QString GameEntity::entityKey(
    const QString& objectName,
    const QMetaObject *metaObject
)
{
    return objectName + "_" + getClassName(metaObject);
}

QString GameEntity::entityKey() const
{
    return GameEntity::entityKey(this->objectName(), this->metaObject());
}

void GameEntity::onObjectNameChanged()
{
    if (GameEntity::anonymusEntities.contains(this))
    {
        GameEntity::anonymusEntities.removeOne(this);
    }
    else
    {
        const QString oldKey = GameEntity::knownEntities.key(this);
        GameEntity::knownEntities.remove(oldKey);
    }
    GameEntity::knownEntities.insert(this->entityKey(), this);
}

QString getClassName(const QMetaObject *metaObject)
{
    QString fullClassName = metaObject->className();

    QStringList classNameParts = fullClassName.split("::");
    return classNameParts.takeLast();
}
