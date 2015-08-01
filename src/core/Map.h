#ifndef CORE_MAP_H
#define CORE_MAP_H

#include <QList>
#include <QString>

#include "core/GameObject.h"

namespace warmonger {
namespace core {

class World;
class MapNode;
class Player;
class Unit;
class Settlement;

class Map :
    public GameObject
{
    Q_OBJECT
    Q_PROPERTY(QString path READ getPath);
    Q_PROPERTY(QString description READ getDescription)
    Q_PROPERTY(QVariant world READ readWorld NOTIFY worldChanged)
    Q_PROPERTY(QVariant mapNodes READ readMapNodes NOTIFY mapNodesChanged)
    Q_PROPERTY(QVariant players READ readPlayers)
    Q_PROPERTY(QVariant units READ readUnits)
    Q_PROPERTY(QVariant settlements READ readSettlements)

public:
    static const QString DefinitionFile;

    Map(QObject *parent);
    ~Map();

    QString getPath() const;
    void setPath(const QString &path);

    QString getDescription()const;
    void setDescription(const QString &description);

    const World * getWorld() const;
    void setWorld(const World *world);
    QVariant readWorld() const;

    QList<const MapNode *> getMapNodes() const;
    QList<MapNode *> getMapNodes();
    void setMapNodes(const QList<MapNode *> &mapNodes);
    QVariant readMapNodes() const;

    QList<const Player *> getPlayers() const;
    QList<Player *> getPlayers();
    void setPlayers(const QList<Player *> &units);
    QVariant readPlayers() const;

    QList<const Unit *> getUnits() const;
    QList<Unit *> getUnits();
    void setUnits(const QList<Unit *> &units);
    QVariant readUnits() const;

    QList<const Settlement *> getSettlements() const;
    QList<Settlement *> getSettlements();
    void setSettlements(const QList<Settlement *> &settlements);
    QVariant readSettlements() const;

signals:
    void worldChanged();
    void mapNodesChanged();

private:
    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;

    QList<MapNode *> mapNodesFromJson(const QJsonObject &obj);
    QJsonObject mapNodesToJson(const QList<MapNode *> &mapNodes) const;

    QString path;
    QString description;
    const World *world;
    QList<MapNode *> mapNodes;
    QList<Player *> players;
    QList<Unit *> units;
    QList<Settlement *> settlements;
};

} // namespace core
} // namespace warmonger

#endif // CORE_MAP_H
