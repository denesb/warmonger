#ifndef CORE_WORLD_RESOURCES_H
#define CORE_WORLD_RESOURCES_H

#include <QJsonObject>
#include <QVariant>
#include <QMap>

namespace warmonger {
namespace core {

class WorldResources :
    public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString basePath READ getBasePath)
    Q_PROPERTY(QVariantMap paths READ readPaths)

public:
    WorldResources(QObject *parent);

    QString getBasePath() const;

    Q_INVOKABLE QString getPath(const QString &resourceName);
    QVariantMap readPaths() const;

    void loadFromJsonFile();
    void fromJson(const QJsonObject &obj);
    QJsonObject toJson() const;

private:
    QVariantMap toQVariantMap(const QMap<QString, QString> &qmap) const;
    QMap<QString, QString> mapFromJson(const QJsonObject &obj) const;
    QJsonObject mapToJson(const QMap<QString, QString> &map) const;

    QString basePath;
    QMap<QString, QString> resourcePaths;
};

} // namespace core
} // namespace warmonger

#endif // CORE_WORLD_RESOURCES_H
