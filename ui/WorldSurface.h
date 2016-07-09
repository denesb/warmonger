#ifndef W_UI_WORLD_SURFACE_H
#define W_UI_WORLD_SURFACE_H

#include <memory>

#include <QColor>
#include <QImage>
#include <QQuickWindow>
#include <QSize>

#include "core/World.h"

namespace warmonger {
namespace ui {

class WorldSurface :
    public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString prefix READ getPrefix NOTIFY prefixChanged)
    Q_PROPERTY(QString displayName READ getDisplayName WRITE setDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(QString description READ getDescription WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QSize tileSize READ getTileSize NOTIFY tileSizeChanged)
    Q_PROPERTY(QColor normalGridColor READ getNormalGridColor WRITE setNormalGridColor NOTIFY normalGridColorChanged)
    Q_PROPERTY(QColor focusGridColor READ getFocusGridColor WRITE setFocusGridColor NOTIFY focusGridColorChanged)

public:
    WorldSurface(const QString& path, core::World *world, QQuickWindow *window, QObject *parent=nullptr);
    ~WorldSurface();

    /**
     * Common path prefix of all paths to surface resources.
     * The prefix contains a trailing /.
     */
    QString getPrefix() const;

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
     * Activate this surface
     *
     * Loads the resource file and registers it with the Qt resource system.
     * This will possibly overwrite any previosly loaded surface. Make sure
     * you call deactivate on the previously activated surface.
     */
    void activate();

    /**
     * Deactivate this surface
     *
     * Unregisters the resources associated with this surface from the Qt
     * resource system.
     */
    void deactivate();

    QSGTexture * getTexture(const QString& path) const;

signals:
    void prefixChanged();
    void displayNameChanged();
    void descriptionChanged();
    void tileWidthChanged();
    void tileHeightChanged();
    void tileSizeChanged();
    void normalGridColorChanged();
    void focusGridColorChanged();

private slots:
    void turnTextureSyncOn();
    void turnTextureSyncOff();

private:
    void parseHeader(const QByteArray& header);
    void uploadTextures();
    void uploadTexture(const QString &pathPrefix, const QObject *object);
    void cleanTextures();

    const QString path;
    QString displayName;
    QString description;
    core::World *world;

    QByteArray resourceData;
    QQuickWindow *window;
    std::map<QString, std::unique_ptr<QSGTexture>> textures;
    bool isTextureSyncOn;
    bool isTextureSyncPending;

    int tileWidth;
    int tileHeight;
    QColor normalGridColor;
    QColor focusGridColor;
    QImage hexMask;
};

} // namespace ui
} // namespace warmonger

#endif // W_UI_WORLD_SURFACE_H
