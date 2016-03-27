#ifndef CORE_WORLD_SURFACE_H
#define CORE_WORLD_SURFACE_H

#include <QColor>
#include <QHash>
#include <QImage>
#include <QSize>
#include <QVariant>

namespace warmonger {
namespace core {

class WorldSurface :
    public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(QSize tileSize READ getTileSize NOTIFY tileSizeChanged);

public:
    explicit WorldSurface(QObject *parent=nullptr);

    QString getDisplayName() const;
    void setDisplayName(const QString &displayName);

    int getTileWidth() const;
    void setTileWidth(int width);

    int getTileHeight() const;
    void setTileHeight(int height);

    QSize getTileSize() const;
    void setTileSize(const QSize &tileSize);

    QMap<QString, QString> getImagePaths() const;
    QString getImagePath(const QString &key) const;

    QImage getImage(const QString &key) const;

    QMap<QString, QString> getColorNames() const;
    QString getColorName(const QString &key) const;

    QColor getColor(const QString &key) const;

    bool hexContains(const QPoint &p) const;
    bool hexContains(const QPointF &p) const;

signals:
    void displayNameChanged();
    void prefixChanged();
    void tileWidthChanged();
    void tileHeightChanged();
    void tileSizeChanged();
    void imagePathsChanged();
    void colorNamesChanged();

private:
    QString displayName;

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
