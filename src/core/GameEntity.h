#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

#include <QJsonDocument>

#include "core/GameObject.h"

namespace warmonger {
namespace core {

class GameEntity :
    public GameObject
{
    Q_OBJECT
    Q_PROPERTY(QString path READ getPath NOTIFY pathChanged);
    Q_PROPERTY(QString fileName READ getFileName NOTIFY fileNameChanged);
    Q_PROPERTY(QString description READ getDescription NOTIFY descriptionChanged);

public:
    GameEntity(QObject *parent);
    virtual ~GameEntity();

    QString getPath() const;
    QString getFileName() const;

    QString getDescription() const;
    void setDescription(const QString &description);

    Q_INVOKABLE virtual void load(const QString &specification);
    Q_INVOKABLE virtual void loadAs(const QString &path);
    Q_INVOKABLE virtual void save() const;
    Q_INVOKABLE virtual void saveAs(const QString &path) const;

    virtual QString specification(const QString &objectName) const = 0;

signals:
    void pathChanged() const;
    void fileNameChanged() const;
    void descriptionChanged() const;
    void loaded() const;
    void saved() const;

protected:
    void setPath(const QString &path);
    void setFileName(const QString &fileName);

    virtual void loadFromFile(const QString &path);
    virtual void saveToFile(const QString &path) const;

    virtual void dataFromJson(const QJsonObject &obj);
    virtual void dataToJson(QJsonObject &obj) const;

    QString path;
    QString fileName;
    QString description;
};

} // namespace warmonger
} // namespace core

#endif // GAME_ENTITY_H
