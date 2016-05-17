#ifndef CORE_WORLD_SURFACE_H
#define CORE_WORLD_SURFACE_H

#include <QColor>
#include <QImage>
#include <QSize>

namespace warmonger {
namespace core {

class WorldSurface :
    public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(QString description READ getDescription WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QSize tileSize READ getTileSize NOTIFY tileSizeChanged)
    Q_PROPERTY(QColor normalGridColor READ getNormalGridColor WRITE setNormalGridColor NOTIFY normalGridColorChanged)
    Q_PROPERTY(QColor focusGridColor READ getFocusGridColor WRITE setFocusGridColor NOTIFY focusGridColorChanged)

public:
    explicit WorldSurface(QObject *parent=nullptr);

    QString getDisplayName() const;
    void setDisplayName(const QString &displayName);

    QString getDescription() const;
    void setDescription(const QString &description);

    int getTileWidth() const;
    void setTileWidth(int width);

    int getTileHeight() const;
    void setTileHeight(int height);

    QSize getTileSize() const;
    void setTileSize(const QSize &tileSize);

    QColor getNormalGridColor() const;
    void setNormalGridColor(const QColor &color);

    QColor getFocusGridColor() const;
    void setFocusGridColor(const QColor &color);

    bool hexContains(const QPoint &p) const;
    bool hexContains(const QPointF &p) const;

    /**
     * Load the WorldSurface from path `path`
     *
     * Loads the resource file and registers it with Qts resource system. This
     * will overwrite any previosly loaded surface possibly.
     * This method relies on the resource file being correct! Missing data may
     * cause silent failures!
     */
    void load(const QString &path);

signals:
    void displayNameChanged();
    void descriptionChanged();
    void tileWidthChanged();
    void tileHeightChanged();
    void tileSizeChanged();
    void normalGridColorChanged();
    void focusGridColorChanged();

private:
    QString displayName;
    QString description;
    int tileWidth;
    int tileHeight;
    QColor normalGridColor;
    QColor focusGridColor;

    QImage hexMask;
};

} // namespace core
} // namespace warmonger

#endif // CORE_WORLD_SURFACE_H
