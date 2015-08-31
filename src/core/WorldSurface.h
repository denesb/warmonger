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
    Q_PROPERTY(QVariantMap bigMap READ readBigMap NOTIFY bigMapChanged);
    Q_PROPERTY(QVariantMap miniMap READ readMiniMap NOTIFY miniMapChanged);
    Q_PROPERTY(QVariantMap style READ readStyle NOTIFY styleChanged);

public:
    WorldSurface(QObject *parent);

    virtual QString specification(const QString &objectName) const;

    QSize getTileSize() const;
    void setTileSize(const QSize &tileSize);

    QMap<QString, QString> getBigMap() const;
    void setBigMap(const QMap<QString, QString> &bigMap);
    QVariantMap readBigMap() const;

    QMap<QString, QString> getMiniMap() const;
    void setMiniMap(const QMap<QString, QString> &miniMap);
    QVariantMap readMiniMap() const;

    QMap<QString, QString> getStyle() const;
    void setStyle(const QMap<QString, QString> &style);
    QVariantMap readStyle() const;

signals:
    void tileSizeChanged();
    void bigMapChanged();
    void miniMapChanged();
    void styleChanged();

private:
    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;

    QVariantMap toQVariantMap(const QMap<QString, QString> &qmap) const;
    QMap<QString, QString> mapFromJson(const QJsonObject &obj) const;
    QJsonObject mapToJson(const QMap<QString, QString> &map) const;

    QSize tileSize;
    QMap<QString, QString> bigMap;
    QMap<QString, QString> miniMap;
    QMap<QString, QString> style;
};

} // namespace core
} // namespace warmonger

#endif // CORE_WORLD_SURFACE_H
