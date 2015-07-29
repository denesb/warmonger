#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <QObject>
#include <QJsonObject>
#include <QVariant>

#include "log/LogStream.h"
#include "core/Exception.h"

namespace warmonger {
namespace core {

class GameObject :
    public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName)

public:
    GameObject(QObject *parent);
    virtual ~GameObject();

    QString getDisplayName() const;
    void setDisplayName(const QString &displayName);

    void fromJson(const QJsonObject &obj);
    QJsonObject toJson() const;

private:
    virtual void dataFromJson(const QJsonObject &obj) = 0;
    virtual void dataToJson(QJsonObject &obj) const = 0;

    QString displayName;
};

} // namespace warmonger
} // namespace core

#endif // GAME_OBJECT_H
