#ifndef W_CORE_ARMY_TYPE_H
#define W_CORE_ARMY_TYPE_H

#include <boost/optional.hpp>

#include <QObject>

#include "core/HierarchyNode.hpp"

namespace warmonger {
namespace core {

class ArmyType : public QObject, public HierarchyNode<ArmyType>
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)

public:
    explicit ArmyType(QObject* parent = nullptr);

    QString getDisplayName() const;
    void setDisplayName(const QString& displayName);

signals:
    void displayNameChanged();

private:
    boost::optional<QString> displayName;
};

} // namespace core
} // namespace warmonger

#endif // W_CORE_ARMY_TYPE_H
