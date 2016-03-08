#ifndef CORE_DAMAGE_TYPE_H
#define CORE_DAMAGE_TYPE_H

#include <QObject>

namespace warmonger {
namespace core {

class DamageType :
    public QObject
{
    Q_OBJECT

public:
    explicit DamageType(QObject *parent=nullptr);
    ~DamageType();

    QString getDisplayName() const;
    void setDisplayName(const QString &displayName);

signals:
    void displayNameChanged();

private:
    QString displayName;
};

} // namespace core
} // namespace warmonger

#endif // CORE_DAMAGE_TYPE_H
