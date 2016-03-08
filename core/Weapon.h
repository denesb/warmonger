#ifndef CORE_WEAPON_H
#define CORE_WEAPON_H

#include <QObject>
#include <QMap>

namespace warmonger {
namespace core {

class DamageType;

class Weapon :
    public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)

public:
    explicit Weapon(QObject *parent=nullptr);

    QString getDisplayName() const;
    void setDisplayName(const QString &displayName);

    int getRange() const;
    void setRange(int range);

    QMap<const DamageType *, int> getDamages() const;
    void setDamages(const QMap<const DamageType *, int> &damages);

    int getDamage(const DamageType * const damageType) const;
    void setDamage(const DamageType * const damageType, int damage);

signals:
    void displayNameChanged();

private:
    QString displayName;
    int range;
    QMap<const DamageType *, int> damages;
};

} // namespace core
} // namespace warmonger

#endif // CORE_WEAPON_H
