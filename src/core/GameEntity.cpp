#include <QFileInfo>

#include "core/GameEntity.h"
#include "core/JsonUtil.hpp"

using namespace warmonger::core;

static const QString module("core.GameEntity");

GameEntity::GameEntity(QObject *parent) :
    GameObject(parent),
    path(),
    fileName(),
    description("")
{
}

GameEntity::~GameEntity()
{
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

void GameEntity::load(const QString &specification)
{
    QFileInfo fileInfo(specification);
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
    this->saveAs(this->path);
    this->saveToFile(this->path);
}

void GameEntity::saveAs(const QString &path) const
{
    this->saveToFile(path);
}

void GameEntity::setPath(const QString &path)
{
    if (!this->path.isNull())
    {
        Exception e(Exception::EntityAlreadyLoaded, {this->objectName()});
        wError(module) << e.getMessage();
        throw e;
    }
    this->path = path;
}

void GameEntity::setFileName(const QString &fileName)
{
    if (!this->fileName.isNull())
    {
        Exception e(Exception::EntityAlreadyLoaded, {this->objectName()});
        wError(module) << e.getMessage();
        throw e;
    }
    this->fileName = fileName;
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
