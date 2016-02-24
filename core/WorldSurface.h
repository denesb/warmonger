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
    Q_PROPERTY(QSize tileSize READ getTileSize NOTIFY tileSizeChanged);
    Q_PROPERTY(QVariantMap imagePaths READ readImagePaths NOTIFY imagePathsChanged);
    Q_PROPERTY(QVariantMap colorNames READ readColorNames NOTIFY colorNamesChanged);

public:
    static const QString fileExtension;

    WorldSurface(QObject *parent=nullptr);

    QString getEntityRelativePath(const QString &name) const;

    int getTileWidth() const;
    void setTileWidth(int width);

    int getTileHeight() const;
    void setTileHeight(int height);

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

    bool hexContains(const QPoint &p) const;
    bool hexContains(const QPointF &p) const;

signals:
    void prefixChanged();
    void tileWidthChanged();
    void tileHeightChanged();
    void tileSizeChanged();
    void imagePathsChanged();
    void colorNamesChanged();

private:
    void dataFromJson(const QJsonObject &obj);
    void dataToJson(QJsonObject &obj) const;

    int tileWidth;
    int tileHeight;

    QHash<QString, QImage> images;
    QMap<QString, QString> imagePaths;
    QHash<QString, QColor> colors;
    QMap<QString, QString> colorNames;

    QImage hexMask;
};

} // namespace core
} // namespace warmonger

#endif // CORE_WORLD_SURFACE_H
