#ifndef CORE_SETTLEMENT_TYPE_H
#define CORE_SETTLEMENT_TYPE_H

#include <vector>

#include <boost/optional.hpp>

#include <QObject>
#include <QVariant>

#include "core/HierarchyNode.hpp"

namespace warmonger {
namespace core {

class UnitType;

/*
 * Can inherit:
 * * displayName
 */
class SettlementType :
    public QObject,
    public HierarchyNode<SettlementType>
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)

public:
    explicit SettlementType(QObject *parent=nullptr);
    ~SettlementType();

    QString getDisplayName() const;
    void setDisplayName(const QString &displayName);

signals:
    void displayNameChanged();

private:
    boost::optional<QString> displayName;
};

} // namespace core
} // namespace warmonger

#endif // CORE_SETTLEMENT_TYPE_H
