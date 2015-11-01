#ifndef CORE_WORLD_SURFACE_H
#define CORE_WORLD_SURFACE_H

#include <QColor>
#include <QHash>
#include <QImage>
#include <QSize>

#include "core/GameEntityPart.h"

namespace warmonger {
namespace core {

class WorldSurface :
    public GameEntityPart
{
    Q_OBJECT
    Q_PROPERTY(QString prefix READ getPrefix NOTIFY prefixChanged);
    Q_PROPERTY(QSize tileSize READ getTileSize NOTIFY tileSizeChanged);
    Q_PROPERTY(QVariantMap imagePaths READ readImagePaths NOTIFY imagePathsChanged);
    Q_PROPERTY(QVariantMap colorNames READ readColorNames NOTIFY colorNamesChanged);

public:
    Q_INVOKABLE WorldSurface(QObject *parent);

    Q_INVOKABLE virtual QString fileExtension() const;

    QString getPrefix() const;

    QSize getTileSize() const;
    void setTileSize(const QSize &tileSize);

    QMap<QString, QString> getImagePaths() const;
    QString getImagePath(const QString &key) const;
    QVariantMap readImagePaths() const;

    QImage getImage(const QString &key) const;

    QMap<QString, QString> getColorNames() const;
    QString getColorName(const QString &key) const;
    QVariantMap readColorNames() const;

    QColor getColor(const QString &key) const;

    Q_INVOKABLE bool hexContains(const QPoint &p) const;
    bool hexContains(const QPointF &p) const;

signals:
    void prefixChanged();
    void tileSizeChanged();
    void imagePathsChanged();
    void colorNamesChanged();

private:
    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;

    QVariantMap toQVariantMap(const QMap<QString, QString> &qmap) const;
    QMap<QString, QString> mapFromJson(const QJsonObject &obj) const;
    QJsonObject mapToJson(const QMap<QString, QString> &map) const;

    QSize tileSize;
    QHash<QString, QImage> images;
    QMap<QString, QString> imagePaths;
    QHash<QString, QColor> colors;
    QMap<QString, QString> colorNames;

    QImage hexMask;
};

} // namespace core
} // namespace warmonger

#endif // CORE_WORLD_SURFACE_H
