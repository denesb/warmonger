#ifndef CORE_CIVILIZATION_H
#define CORE_CIVILIZATION_H

#include <map>
#include <vector>

#include <QObject>
#include <QVariant>

namespace warmonger {
namespace core {

class UnitType;

class Civilization : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(QVariantList unitTypes READ readUnitTypes NOTIFY unitTypesChanged)

public:
    explicit Civilization(QObject* parent = nullptr);

    QString getDisplayName() const;
    void setDisplayName(const QString& displayName);

    std::vector<UnitType*> getUnitTypes() const;
    QVariantList readUnitTypes() const;
    void setUnitTypes(const std::vector<UnitType*>& unitTypes);
    void addUnitType(UnitType* unitType);

signals:
    void displayNameChanged();
    void unitTypesChanged();

private:
    QString displayName;
    std::vector<UnitType*> unitTypes;
};

} // namespace core
} // namespace warmonger

#endif // CORE_CIVILIZATION_H
