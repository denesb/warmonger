#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <QObject>
#include <QJsonObject>

namespace warmonger {
namespace core {

class GameObject :
    public QObject
{
    Q_OBJECT

public:
    GameObject(QObject *parent = nullptr);
    virtual ~GameObject();

    virtual void fromJson(const QJsonObject &obj) = 0;
    virtual QJsonObject toJson() const = 0;
};

}; // namespace warmonger
}; // namespace core

#endif // GAME_OBJECT_H
