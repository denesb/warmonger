#ifndef GAME_ENTITY_H
#define GAME_ENTITY_H

#include "core/GameObject.h"

namespace warmonger {
namespace core {

class GameObject;

class GameEntity :
    public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(QString description READ getDescription NOTIFY descriptionChanged);

public:
    GameEntity(QObject *parent);
    virtual ~GameEntity();

    QString getDescription() const;
    void setDescription(const QString &description);

    QString getDisplayName() const;
    void setDisplayName(const QString &displayName);

    void fromJson(const QJsonObject &obj);
    QJsonObject toJson() const;

signals:
    void displayNameChanged();
    void descriptionChanged();

protected:
    virtual void dataFromJson(const QJsonObject &obj) = 0;
    virtual void dataToJson(QJsonObject &obj) const = 0;

private:
    QString displayName;
    QString description;
};

} // namespace warmonger
} // namespace core

#endif // GAME_ENTITY_H
