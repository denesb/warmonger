#ifndef CORE_UNIT_LEVEL_H
#define CORE_UNIT_LEVEL_H

#include "core/GameObject.h"

namespace warmonger {
namespace core {

class UnitLevel :
    public GameObject
{
    Q_OBJECT;
    Q_PROPERTY(int index READ getIndex WRITE setIndex NOTIFY indexChanged)
    Q_PROPERTY(int experiencePoints READ getExperiencePoints WRITE setExperiencePoints NOTIFY experiencePointsChanged)

public:
    UnitLevel(QObject *parent=nullptr);
    ~UnitLevel();

    int getIndex() const;
    void setIndex(int index);

    int getExperiencePoints() const;
    void setExperiencePoints(int experiencePoints);

signals:
    void experiencePointsChanged();
    void indexChanged();

private:
    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;

    int experiencePoints;
    int index;
};

} // namespace core
} // namespace warmonger

#endif // CORE_UNIT_LEVEL_H
