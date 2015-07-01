#ifndef CORE_WORLD_META_H
#define CORE_WORLD_META_H

#include <QObject>
#include <QString>
#include <QJsonObject>

namespace core {

class WorldMeta :
    public QObject
{
    Q_OBJECT

public:
    WorldMeta(QObject *parent = nullptr);

    QString getDisplayName() const;
    void setDisplayName(const QString &displayName);

    QString getDescription() const;
    void setDescription(const QString &description);

    void fromJson(const QJsonObject &obj);
    QJsonObject toJson() const;

private:
    QString name;
    QString displayName;
    QString description;
};

}; // namespace core

#endif // CORE_GAME_ENGINE_H
