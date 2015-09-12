#include <QFileInfo>

#include "core/GameEntityPart.h"
#include "core/JsonUtil.h"

using namespace warmonger::core;

static const QString category("core");

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

void GameEntityPart::load()
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

void GameEntityPart::loadAs(const QString &path)
{
    QFileInfo fileInfo(path);
    this->setPath(fileInfo.absolutePath());
    this->setFileName(fileInfo.fileName());

    this->loadFromFile(fileInfo.absoluteFilePath());
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
    if (!this->path.isNull())
    {
        wError(category) << "Entity " << this->objectName() << " already loaded";
        throw Exception(Exception::EntityAlreadyLoaded);
    }
    this->path = path;

    emit this->pathChanged();
}

void GameEntityPart::setFileName(const QString &fileName)
{
    if (!this->fileName.isNull())
    {
        wError(category) << "Entity " << this->objectName() << " already loaded";
        throw Exception(Exception::EntityAlreadyLoaded);
    }
    this->fileName = fileName;

    emit this->fileNameChanged();
}

QString GameEntityPart::getPrefixedFileName() const
{
    static const QString pfnTemplate{"%1:%2.%3"};

    const QMetaObject *metaObject = this->metaObject();
    QString className = getClassName(metaObject->className());

    return pfnTemplate.arg(
        className,
        this->objectName(),
        this->getFileExtension()
    );
}

void GameEntityPart::loadFromFile(const QString &path)
{
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
