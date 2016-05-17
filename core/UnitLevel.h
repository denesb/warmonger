#ifndef CORE_UNIT_LEVEL_H
#define CORE_UNIT_LEVEL_H

#include <QObject>

namespace warmonger {
namespace core {

class UnitLevel :
    public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(int index READ getIndex WRITE setIndex NOTIFY indexChanged)
    Q_PROPERTY(int experiencePoints READ getExperiencePoints WRITE setExperiencePoints NOTIFY experiencePointsChanged)

public:
    UnitLevel(QObject *parent=nullptr);
    ~UnitLevel();

    QString getDisplayName() const;
    void setDisplayName(const QString &displayName);

    int getIndex() const;
    void setIndex(int index);

    int getExperiencePoints() const;
    void setExperiencePoints(int experiencePoints);

signals:
    void displayNameChanged();
    void experiencePointsChanged();
    void indexChanged();

private:
    QString displayName;
    int experiencePoints;
    int index;
};

} // namespace core
} // namespace warmonger

#endif // CORE_UNIT_LEVEL_H
