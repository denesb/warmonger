#ifndef CORE_WEAPON_CLASS_H
#define CORE_WEAPON_CLASS_H

#include <QObject>

namespace warmonger {
namespace core {

class WeaponClass :
    public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)

public:
    explicit WeaponClass(QObject *parent=nullptr);

    QString getDisplayName() const;
    void setDisplayName(const QString &displayName);

signals:
    void displayNameChanged();

private:
    QString displayName;
};

} // namespace core
} // namespace warmonger

#endif // CORE_WEAPON_CLASS_H
