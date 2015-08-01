#ifndef CORE_WORLD_H
#define CORE_WORLD_H

#include <QList>

#include "core/GameObject.h"

namespace warmonger {
namespace core {

class TerrainType;
class UnitClass;
class DamageType;
class Weapon;
class Armor;
class UnitType;
class SettlementType;
class Faction;

class World :
    public GameObject
{
    Q_OBJECT
    Q_PROPERTY(QString path READ getPath NOTIFY pathChanged);
    Q_PROPERTY(QString description READ getDescription);
    Q_PROPERTY(QVariant terrainTypes READ readTerrainTypes NOTIFY terrainTypesChanged)
    Q_PROPERTY(QVariant unitTypes READ readUnitTypes NOTIFY unitTypesChanged)
    Q_PROPERTY(QVariant settlementTypes READ readSettlementTypes NOTIFY settlementTypesChanged)
    Q_PROPERTY(QVariantMap resourcePaths READ readResourcePaths NOTIFY resourcePathsChanged);

public:
    static const QString DefinitionFile;

    World(QObject *parent);

    QString getPath() const;
    void setPath(const QString &path);
 
    QString getDescription() const;
    void setDescription(const QString &description);

    QList<const TerrainType *> getTerrainTypes() const;
    QList<TerrainType *> getTerrainTypes();
    void setTerrainTypes(const QList<TerrainType *> &terrainTypes);
    QVariant readTerrainTypes() const;

    QList<const UnitClass *> getUnitClasses() const;
    QList<UnitClass *> getUnitClasses();
    void setUnitClasses(const QList<UnitClass *> &unitClasses);

    QList<const DamageType *> getDamageTypes() const;
    QList<DamageType *> getDamageTypes();
    void setDamageTypes(const QList<DamageType *> &damageTypes);

    QList<const Armor *> getArmors() const;
    QList<Armor *> getArmors();
    void setArmors(const QList<Armor *> &armors);

    QList<const Weapon *> getWeapons() const;
    QList<Weapon *> getWeapons();
    void setWeapons(const QList<Weapon *> &weapons);

    QList<const UnitType *> getUnitTypes() const;
    QList<UnitType *> getUnitTypes();
    void setUnitTypes(const QList<UnitType *> &unitTypes);
    QVariant readUnitTypes() const;

    QList<const SettlementType *> getSettlementTypes() const;
    QList<SettlementType *> getSettlementTypes();
    void setSettlementTypes(const QList<SettlementType *> &settlementTypes);
    QVariant readSettlementTypes() const;

    QList<const Faction *> getFactions() const;
    QList<Faction *> getFactions();
    void setFactions(const QList<Faction *> &factions);

    QMap<QString, QString> getResourcePaths() const;
    void setResourcePaths(const QMap<QString, QString> &resourcePaths);
    QVariantMap readResourcePaths() const;
    Q_INVOKABLE QString getResourcePath(const QString &resourceName) const;

signals:
    void pathChanged();
    void terrainTypesChanged();
    void unitTypesChanged();
    void settlementTypesChanged();
    void resourcePathsChanged();

private:
    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;

    QJsonObject loadFromJsonFile(const QString &path);
    QVariantMap toQVariantMap(const QMap<QString, QString> &qmap) const;
    QMap<QString, QString> mapFromJson(const QJsonObject &obj) const;
    QJsonObject mapToJson(const QMap<QString, QString> &map) const;

    QString path;
    QString description;
    QList<TerrainType *> terrainTypes;
    QList<UnitClass *> unitClasses;
    QList<DamageType *> damageTypes;
    QList<Weapon *> weapons;
    QList<Armor *> armors;
    QList<UnitType *> unitTypes;
    QList<SettlementType *> settlementTypes;
    QList<Faction *> factions;

    QMap<QString, QString> resourcePaths;
};

} // namespace core
} // namespace warmonger

#endif // CORE_WORLD_H
