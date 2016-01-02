#include <QDir>
#include <QFileInfo>

#include "core/EntityManager.h"
#include "core/GameEntityPart.h"
#include "core/QJsonUtil.h"

using namespace warmonger::core;

static const QString loggerName("core.GameEntityPart");

GameEntityPart::GameEntityPart(QObject *parent) :
    QObject(parent),
    path(),
    fileName(),
    displayName(""),
    description("")
{
}

GameEntityPart::~GameEntityPart()
{
}

QString GameEntityPart::getPath() const
{
    return this->path;
}

QString GameEntityPart::getFileName() const
{
    return this->fileName;
}

QString GameEntityPart::getDescription() const
{
    return this->description;
}

void GameEntityPart::setDescription(const QString &description)
{
    if (this->description != description)
    {
        this->description = description;
        emit descriptionChanged();
    }
}

QString GameEntityPart::getDisplayName() const
{
    return this->displayName;
}

void GameEntityPart::setDisplayName(const QString &displayName)
{
    if (this->displayName != displayName)
    {
        this->displayName = displayName;
        emit displayNameChanged();
    }
}

void GameEntityPart::load(const GameEntity *entity, const QString &name)
{
    const EntityManager *em = EntityManager::getInstance();
    const QString entityPath = em->getEntityPath(entity);
    const QFileInfo entityFile(entityPath);

    const QString relativePath = this->getEntityRelativePath(name);

    const QString path(entityFile.absolutePath() + '/' + relativePath);

    this->loadFromFile(QDir::cleanPath(path));
}

void GameEntityPart::loadAs(const QString &path)
{
    this->loadFromFile(path);
}

void GameEntityPart::save() const
{
    const QString path(this->path + "/" + this->fileName);
    this->saveToFile(path);
}

void GameEntityPart::saveAs(const QString &path) const
{
    this->saveToFile(path);
}

void GameEntityPart::fromJson(const QJsonObject &obj)
{
    this->setObjectName(obj["objectName"].toString());
    this->displayName = obj["displayName"].toString();

    this->dataFromJson(obj);
}

QJsonObject GameEntityPart::toJson() const
{
	QJsonObject obj;
	obj["objectName"] = this->objectName();
	obj["displayName"] = this->displayName;

    this->dataToJson(obj);

	return std::move(obj);
}

void GameEntityPart::setPath(const QString &path)
{
    this->path = path;

    emit this->pathChanged();
}

void GameEntityPart::setFileName(const QString &fileName)
{
    this->fileName = fileName;

    emit this->fileNameChanged();
}

void GameEntityPart::loadFromFile(const QString &path)
{
    QFileInfo fileInfo(path);
    this->setPath(fileInfo.absolutePath());
    this->setFileName(fileInfo.fileName());

    QJsonDocument doc = loadJsonDocument(path);
    this->fromJson(doc.object());

    emit this->loaded();
}

void GameEntityPart::saveToFile(const QString &path) const
{
    QJsonDocument doc(this->toJson());
    saveJsonDocument(path, doc);

    emit this->saved();
}
