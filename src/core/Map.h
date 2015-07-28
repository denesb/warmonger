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
    Q_PROPERTY(QString description READ getDescription)
    Q_PROPERTY(QVariant world READ readWorld)
    Q_PROPERTY(QVariantList mapNodes READ readMapNodes)
    Q_PROPERTY(QVariantList players READ readPlayers)
    Q_PROPERTY(QVariantList units READ readUnits)
    Q_PROPERTY(QVariantList settlements READ readSettlements)

public:
    static const QString DefinitionFile;

    Map(QObject *parent);
    ~Map();

    QString getDescription()const;
    void setDescription(const QString &description);

    const World * getWorld() const;
    void setWorld(const World *world);
    QVariant readWorld() const;

    QList<const MapNode *> getMapNodes() const;
    QList<MapNode *> getMapNodes();
    void setMapNodes(const QList<MapNode *> &mapNodes);
    QVariantList readMapNodes() const;

    QList<const Player *> getPlayers() const;
    QList<Player *> getPlayers();
    void setPlayers(const QList<Player *> &units);
    QVariantList readPlayers() const;

    QList<const Unit *> getUnits() const;
    QList<Unit *> getUnits();
    void setUnits(const QList<Unit *> &units);
    QVariantList readUnits() const;

    QList<const Settlement *> getSettlements() const;
    QList<Settlement *> getSettlements();
    void setSettlements(const QList<Settlement *> &settlements);
    QVariantList readSettlements() const;

private:
    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;

    QList<MapNode *> mapNodesFromJson(const QJsonObject &obj);
    QJsonObject mapNodesToJson(const QList<MapNode *> &mapNodes) const;

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
