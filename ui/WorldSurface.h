#ifndef W_UI_WORLD_SURFACE_H
#define W_UI_WORLD_SURFACE_H

#include <memory>

#include <QColor>
#include <QImage>
#include <QSize>
#include <QUrl>

#include "core/World.h"
#include "utils/Utils.h"

class QQuickWindow;
class QSGTexture;

namespace warmonger {
namespace ui {

class WorldSurface : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString prefix READ getPrefix NOTIFY prefixChanged)
    Q_PROPERTY(QString displayName READ getDisplayName NOTIFY displayNameChanged)
    Q_PROPERTY(QString description READ getDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QSize tileSize READ getTileSize NOTIFY tileSizeChanged)
    Q_PROPERTY(QColor normalGridColor READ getNormalGridColor NOTIFY normalGridColorChanged)
    Q_PROPERTY(QColor focusGridColor READ getFocusGridColor NOTIFY focusGridColorChanged)

public:
    WorldSurface(const QString& path, core::World* world, QObject* parent = nullptr);
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

    /**
     * Get the QSGTexture for the object (e.g. a UnitType) and window
     *
     * If the the texture is not found for `window' it is created.
     * If the lookup and creation fails nullptr will be returned.
     *
     * Warning: Only call this function on the rendering thread (e.g.
     * from the `updatePaintedNode' function!
     *
     * Warning: The returned texture is owned by the surface, do not delete
     * it and do not allow any QSGNodes to take ownership!
     *
     * @param[in] const QObject* object the object to get the texture for
     * @param[in] QQuickWindow* window the window to which the texture belongs
     * to
     *
     * @returns QSGTexture*
     */
    QSGTexture* getTexture(const QObject* object, QQuickWindow* window);

    /**
     * Get the QSGTexture for the key (e.g. a UnitType) and window
     *
     * For more details see getTexture(const QObject*, QQuickWindow*).
     *
     * @param[in] const QString& key the key to get the texture for
     * @param[in] QQuickWindow* window the window to which the texture belongs
     * to
     *
     * @returns QSGTexture*
     */
    QSGTexture* getTexture(const QString& key, QQuickWindow* window);

    Q_INVOKABLE QUrl getImageUrl(QObject* object) const;

signals:
    void prefixChanged();
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
    core::World* world;

    QByteArray resourceData;
    std::map<std::pair<const QString, QQuickWindow*>, std::unique_ptr<QSGTexture, utils::DelayedQObjectDeleter>>
        staticTextures;
    std::map<std::pair<const QObject*, QQuickWindow*>, std::unique_ptr<QSGTexture, utils::DelayedQObjectDeleter>>
        objectTextures;

    int tileWidth;
    int tileHeight;
    QColor normalGridColor;
    QColor focusGridColor;
    QImage hexMask;
};

} // namespace ui
} // namespace warmonger

#endif // W_UI_WORLD_SURFACE_H
