#ifndef GAME_ENTITY_PART_H
#define GAME_ENTITY_PART_H

#include <QJsonDocument>

#include "core/GameObject.h"

namespace warmonger {
namespace core {

class GameEntityPart :
    public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString path READ getPath NOTIFY pathChanged);
    Q_PROPERTY(QString fileName READ getFileName NOTIFY fileNameChanged);
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(QString description READ getDescription NOTIFY descriptionChanged);

public:
    GameEntityPart(QObject *parent);
    virtual ~GameEntityPart();

    QString getPath() const;
    QString getFileName() const;

    Q_INVOKABLE virtual QString getFileExtension() const = 0;

    QString getDescription() const;
    void setDescription(const QString &description);

    QString getDisplayName() const;
    void setDisplayName(const QString &displayName);

    Q_INVOKABLE virtual void loadAs(const QString &path);
    Q_INVOKABLE virtual void save() const;
    Q_INVOKABLE virtual void saveAs(const QString &path) const;

    void fromJson(const QJsonObject &obj);
    QJsonObject toJson() const;

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

    virtual void loadFromFile(const QString &path);
    virtual void saveToFile(const QString &path) const;

    virtual void dataFromJson(const QJsonObject &obj) = 0;
    virtual void dataToJson(QJsonObject &obj) const = 0;

private:
    QString path;
    QString fileName;
    QString displayName;
    QString description;
};

} // namespace warmonger
} // namespace core

#endif // GAME_ENTITY_PART_H
