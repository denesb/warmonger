#include <QObject>
#include <QString>
#include <QJsonObject>

#ifndef CORE_WORLD_META_H
#define CORE_WORLD_META_H

namespace core {

class WorldMeta :
    public QObject
{
    Q_OBJECT

public:
    Q_PROPERTY(QString name        READ getName)
    Q_PROPERTY(QString displayName READ getDisplayName)
    Q_PROPERTY(QString descripion  READ getDescription)
    Q_PROPERTY(QString path        READ getPath)

    WorldMeta(const QString &path, QObject *parent = nullptr);

    QString getName() const;
    QString getDisplayName() const;
    QString getDescription() const;
    QString getPath() const;

private:
    void init(const QJsonObject &metaObject);

    QString name;
    QString displayName;
    QString description;
    QString path;
};

}; // namespace core

#endif // CORE_GAME_ENGINE_H
