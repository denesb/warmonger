#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

#include <QJsonDocument>

#include "core/GameObject.h"

namespace warmonger {
namespace core {

class GameObject;

class GameEntity :
    public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString path READ getPath NOTIFY pathChanged);
    Q_PROPERTY(QString fileName READ getFileName NOTIFY fileNameChanged);
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(QString description READ getDescription NOTIFY descriptionChanged);

public:
    GameEntity();
    virtual ~GameEntity();

    QString getPath() const;
    QString getFileName() const;

    Q_INVOKABLE virtual QString fileExtension() const = 0;

    QString getDescription() const;
    void setDescription(const QString &description);

    QString getDisplayName() const;
    void setDisplayName(const QString &displayName);

    Q_INVOKABLE virtual void load();
    Q_INVOKABLE virtual void loadAs(const QString &path);
    Q_INVOKABLE virtual void save() const;
    Q_INVOKABLE virtual void saveAs(const QString &path) const;

    void fromJson(const QJsonObject &obj);
    QJsonObject toJson() const;

    static QObject * getOwner();
    static void setOwner(QObject *owner);

    Q_INVOKABLE static GameEntity * get(
        const QString &objectName,
        const QMetaObject *metaObject
    );

    Q_INVOKABLE static GameEntity * load(
        const QString &objectName,
        const QMetaObject *metaObject
    );

    virtual GameObject *resolveReference(const QString &objectName) const;

signals:
    void pathChanged();
    void fileNameChanged();
    void displayNameChanged();
    void descriptionChanged();
    void loaded();
    void saved() const;

protected:
    void setPath(const QString &path);
    void setFileName(const QString &fileName);

    QString getPrefixedFileName() const;

    virtual void loadFromFile(const QString &path);
    virtual void saveToFile(const QString &path) const;

    virtual void dataFromJson(const QJsonObject &obj) = 0;
    virtual void dataToJson(QJsonObject &obj) const = 0;

    static QString entityKey(
        const QString& objectName,
        const QMetaObject *metaObject
    );

    QString entityKey() const;

private slots:
    void onObjectNameChanged();

private:
    QString path;
    QString fileName;
    QString displayName;
    QString description;

    static QObject *owner;
    static QMap<QString, GameEntity *> knownEntities;
    static QList<GameEntity *> anonymusEntities;
};

} // namespace warmonger
} // namespace core

#endif // GAME_ENTITY_H
