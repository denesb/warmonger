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
    Q_PROPERTY(QString displayName READ getDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(QString description READ getDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QSize tileSize READ getTileSize NOTIFY tileSizeChanged)
    Q_PROPERTY(QColor normalGridColor READ getNormalGridColor NOTIFY normalGridColorChanged)
    Q_PROPERTY(QColor focusGridColor READ getFocusGridColor NOTIFY focusGridColorChanged)

public:
    WorldSurface(const QString& path, core::World* world, QQuickWindow *window, QObject *parent=nullptr);
    ~WorldSurface();

    /**
     * Common path prefix of all paths to surface resources.
     * The prefix contains a trailing /.
     */
    QString getPrefix() const;

    core::World* getWorld() const;

    QString getDisplayName() const;

    QString getDescription() const;

    int getTileWidth() const;

    int getTileHeight() const;

    QSize getTileSize() const;

    QColor getNormalGridColor() const;

    QColor getFocusGridColor() const;

    bool hexContains(const QPoint& p) const;
    bool hexContains(const QPointF& p) const;

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

    QSGTexture* getTexture(const QObject* object) const;

    QUrl getImageUrl(const QObject* object) const;

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
    void uploadTexture(const QObject *object);
    void cleanTextures();

    const QString path;
    QString displayName;
    QString description;
    core::World* world;

    QByteArray resourceData;
    QQuickWindow* window;
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
