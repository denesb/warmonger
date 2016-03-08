#ifndef CORE_SETTLEMENT_H
#define CORE_SETTLEMENT_H

#include <QObject>

#include "core/MapNode.h"
#include "core/Player.h"
#include "core/SettlementType.h"

namespace warmonger {
namespace core {

class Settlement :
    public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(SettlementType *type READ getType WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(MapNode *mapNode READ getMapNode WRITE setMapNode NOTIFY mapNodeChanged)
    Q_PROPERTY(Player *owner READ getOwner WRITE setOwner NOTIFY ownerChanged)
    Q_PROPERTY(QVariantList recruits READ readRecruits NOTIFY recruitsChanged);

public:
    explicit Settlement(QObject *parent=nullptr);
    ~Settlement();

    QString getDisplayName() const;
    void setDisplayName(const QString &displayName);

    SettlementType * getType() const;
    void setType(SettlementType *type);

    MapNode * getMapNode() const;
    void setMapNode(MapNode *mapNode);

    Player * getOwner() const;
    void setOwner(Player *owner);

    /**
     * Convenience method to get all recruitable unit-types.
     *
     * This is a merge of the owner's faction's recruits for this
     * settlement-type and the settlement-type's own recruits (those
     * available for all factions).
     * If it has no owner only the settlement-type's recruits will be
     * returned.
     */
    QList<UnitType *> getRecruits() const;
    QVariantList readRecruits() const;

signals:
    void displayNameChanged();
    void typeChanged();
    void mapNodeChanged();
    void ownerChanged();
    void recruitsChanged();

private:
    QString displayName;
    SettlementType *type;
    MapNode *mapNode;
    Player *owner;
};

} // namespace core
} // namespace warmonger

#endif // CORE_SETTLEMENT_H
