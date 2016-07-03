#ifndef UI_WORLD_SURFACE_H
#define UI_WORLD_SURFACE_H

#include <QColor>
#include <QImage>
#include <QSize>

namespace warmonger {
namespace ui {

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
    explicit WorldSurface(const QString& path, QObject *parent=nullptr);
    ~WorldSurface();

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
     * Activate the this surface
     *
     * Loads the resource file and registers it with Qts resource system. This
     * will overwrite any previosly loaded surface possibly.
     * This method relies on the resource file being correct! Missing data may
     * possibly cause silent failures!
     */
    void activate();

    /**
     * Deactivate the this surface
     *
     * Unregisters the resources associated witht this surface from the Qt
     * resource system.
     */
    void deactivate();

signals:
    void displayNameChanged();
    void descriptionChanged();
    void tileWidthChanged();
    void tileHeightChanged();
    void tileSizeChanged();
    void normalGridColorChanged();
    void focusGridColorChanged();

private:
    void parseHeader(const QByteArray& header);

    const QString path;
    QString displayName;
    QString description;

    QByteArray resourceData;

    int tileWidth;
    int tileHeight;
    QColor normalGridColor;
    QColor focusGridColor;
    QImage hexMask;
};

} // namespace ui
} // namespace warmonger

#endif // UI_WORLD_SURFACE_H
