#ifndef CORE_WORLD_SURFACE_H
#define CORE_WORLD_SURFACE_H

#include <QSize>

#include "core/GameEntity.h"

namespace warmonger {
namespace core {

class WorldSurface :
    public GameEntity
{
    Q_OBJECT
    Q_PROPERTY(QSize tileSize READ getTileSize NOTIFY tileSizeChanged);
    Q_PROPERTY(QVariantMap gameMap READ readGameMap NOTIFY gameMapChanged);
    Q_PROPERTY(QVariantMap miniMap READ readMiniMap NOTIFY miniMapChanged);

public:
    WorldSurface(QObject *parent);

    virtual QString specification(const QString &objectName) const;

    QSize getTileSize() const;
    void setTileSize(const QSize &tileSize);

    QMap<QString, QString> getGameMap() const;
    void setGameMap(const QMap<QString, QString> &gameMap);
    QVariantMap readGameMap() const;

    QMap<QString, QString> getMiniMap() const;
    void setMiniMap(const QMap<QString, QString> &miniMap);
    QVariantMap readMiniMap() const;

signals:
    void tileSizeChanged() const;
    void gameMapChanged() const;
    void miniMapChanged() const;

private:
    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;

    QVariantMap toQVariantMap(const QMap<QString, QString> &qmap) const;
    QMap<QString, QString> mapFromJson(const QJsonObject &obj) const;
    QJsonObject mapToJson(const QMap<QString, QString> &map) const;

    QSize tileSize;
    QMap<QString, QString> gameMap;
    QMap<QString, QString> miniMap;
};

} // namespace core
} // namespace warmonger

#endif // CORE_WORLD_SURFACE_H
