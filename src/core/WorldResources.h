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
    Q_PROPERTY(QVariantMap terrainTypePaths READ readTerrainTypePaths)
    Q_PROPERTY(QVariantMap settlementTypePaths READ readSettlementTypePaths)
    Q_PROPERTY(QVariantMap unitTypePaths READ readUnitTypePaths)

public:
    WorldResources(QObject *parent);

    QString getTerrainTypePath(const QString &terrainTypeName);
    QVariantMap readTerrainTypePaths() const;

    QString getSettlementTypePath(const QString &settlementTypeName);
    QVariantMap readSettlementTypePaths() const;

    QString getUnitTypePath(const QString &unitTypeName);
    QVariantMap readUnitTypePaths() const;

    void loadFromJsonFile();
    void fromJson(const QJsonObject &obj);
    QJsonObject toJson() const;

private:
    QVariantMap toQVariantMap(const QMap<QString, QString> &qmap) const;
    QMap<QString, QString> mapFromJson(const QJsonObject &obj) const;
    QJsonObject mapToJson(const QMap<QString, QString> &map) const;

    QString basePath;
    QMap<QString, QString> terrainTypePaths;
    QMap<QString, QString> settlementTypePaths;
    QMap<QString, QString> unitTypePaths;
};

} // namespace core
} // namespace warmonger

#endif // CORE_WORLD_RESOURCES_H
