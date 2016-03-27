#ifndef CORE_ARMOR_H
#define CORE_ARMOR_H

#include <map>

#include <QObject>

namespace warmonger {
namespace core {

class DamageType;

class Armor :
    public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)

public:
    explicit Armor(QObject *parent=nullptr);
    ~Armor();

    QString getDisplayName() const;
    void setDisplayName(const QString &displayName);

    std::map<const DamageType *, int> getDefenses() const;
    void setDefenses(const std::map<const DamageType *, int> &damages);

    int getDefense(const DamageType * const damageType) const;
    void setDefense(const DamageType * const damageType, int defense);

signals:
    void displayNameChanged();

private:
    std::map<const DamageType *, int> defenses;

    QString displayName;
};

} // namespace core
} // namespace warmonger

#endif // CORE_ARMOR_H
