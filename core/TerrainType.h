#ifndef CORE_TERRAIN_TYPE_H
#define CORE_TERRAIN_TYPE_H

#include <QObject>

namespace warmonger {
namespace core {

class TerrainType : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)

public:
    explicit TerrainType(QObject* parent = nullptr);
    ~TerrainType();

    QString getDisplayName() const;
    void setDisplayName(const QString& displayName);

signals:
    void displayNameChanged();

private:
    QString displayName;
};

} // namespace core
} // namespace warmonger

#endif // CORE_TERRAIN_TYPE_H
