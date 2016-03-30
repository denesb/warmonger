#ifndef CORE_WEAPON_TYPE_H
#define CORE_WEAPON_TYPE_H

#include <QObject>

#include "core/WeaponClass.h"

namespace warmonger {
namespace core {

class WeaponType :
    public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)

public:
    explicit WeaponType(QObject *parent=nullptr);

    QString getDisplayName() const;
    void setDisplayName(const QString &displayName);

    WeaponClass * getClass() const;
    void setClass(WeaponClass *klass);

signals:
    void displayNameChanged();
    void classChanged();

private:
    QString displayName;
    WeaponClass *klass;
};

} // namespace core
} // namespace warmonger

#endif // CORE_WEAPON_TYPE_H
