#ifndef W_CORE_TERRAIN_TYPE_H
#define W_CORE_TERRAIN_TYPE_H

#include <boost/optional.hpp>

#include <QObject>

#include "core/HierarchyNode.hpp"

namespace warmonger {
namespace core {

class TerrainType : public QObject, public HierarchyNode<TerrainType>
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)

public:
    explicit TerrainType(QObject* parent = nullptr);

    QString getDisplayName() const;
    void setDisplayName(const QString& displayName);

signals:
    void displayNameChanged();

private:
    boost::optional<QString> displayName;
};

} // namespace core
} // namespace warmonger

#endif // W_CORE_TERRAIN_TYPE_H
