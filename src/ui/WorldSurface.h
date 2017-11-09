/** \file
 * WorldSurface class.
 *
 * \copyright (C) 2015-2017 Botond Dénes
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

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

/**
 * Graphical representation of a world.
 *
 * Contains all graphical assets needed to display the world. It contains a
 * graphical representation for all objects in the world that have one.
 * The world-surface manages all aspects of the assets. Loading them from disk,
 * and providing them to the application in different forms.
 * Clients of the world-surface can request assets in two forms:
 * - QSGTexture - used in the qquick scene-graph
 * - file path or url - used in directly creating QImages or passing it as
 *   image source to other components
 *
 * QSGTextures should only be used on the scene-graph rendering thread. The
 * world-surface takes care of creating and destroying them.
 * Paths and urls should only be used with Qt's file classes (QFile, QImage,
 * etc.).
 *
 * The world-surface also manages other information related to the visual
 * representation of the world, like the tile-size, border-colors, etc.
 *
 * \see warmonger::core::World
 */
class WorldSurface : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName NOTIFY nameChanged)
    Q_PROPERTY(QString description READ getDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QSize tileSize READ getTileSize NOTIFY tileSizeChanged)

public:
    class Storage;

    /**
     * Static images used in the drawing of the world.
     */
    struct Image
    {
        static const QString Logo;
    };

    /**
     * Constructs an inactive world-surface.
     *
     * Loads the metadata from the disk. The metadata contains the
     * name and description.
     * Call WorldSurface::activate() to activate the surface.
     *
     * \param path the path to the surface package file (.wsp) or
     *  surface description file (.wsd).
     * \param world the world this surface belongs to
     * \param parent the parent QObject
     */
    WorldSurface(const QString& path, core::World* world, QObject* parent = nullptr);

    /**
     * Deactivates (if it was active) and destroys the surface.
     */
    ~WorldSurface();

    /**
     * Get the world.
     *
     * \return the world
     */
    core::World* getWorld() const
    {
        return this->world;
    }

    /**
     * Get the name.
     *
     * \return the name
     */
    QString getName() const
    {
        return this->name;
    }

    /**
     * Get the description.
     *
     * \return the description
     */
    QString getDescription() const
    {
        return this->description;
    }

    /**
     * Get the tile-width.
     *
     * \return the tile-width
     */
    int getTileWidth() const
    {
        return this->tileWidth;
    }

    /**
     * Get the tile-height.
     *
     * \return the tile-height
     */
    int getTileHeight() const
    {
        return this->tileHeight;
    }

    /**
     * Get the tile-size.
     *
     * \return the tile-size
     */
    QSize getTileSize() const
    {
        return QSize(this->tileWidth, this->tileHeight);
    }

    /**
     * Does the hexagon contain the point?
     *
     * Used to determine wether a hexagon contains the mouse pointer with
     * pixel-perfect precision. The passed in point has to be normalised,
     * that is the hexagon in question is considered to be at position (0,0)
     * with the bounding rectangle QRect(0, 0, tile-width, tile-height).
     *
     * \param p the point
     *
     * \returns wether the point is in the hexagon or not
     */
    bool hexContains(const QPoint& p) const;

    /**
     * Does the hexagon contain the point?
     *
     * QPointF overload.
     * \see WorldSurface::hexContains(const QPoint&) const
     *
     * \param p the point
     *
     * \returns wether the point is in the hexagon or not
     */
    bool hexContains(const QPointF& p) const;

    /**
     * Activate the surface.
     *
     * Loads the resource file and registers it with the Qt resource system.
     * This will possibly overwrite any previosly loaded surface. Make sure
     * you call deactivate on the previously activated surface.
     */
    void activate();

    /**
     * Deactivate the surface.
     *
     * Unregisters the resources associated with this surface from the Qt
     * resource system.
     */
    void deactivate();

    /**
     * Get the QSGTexture for the object and window.
     *
     * If the the texture is not found for `window' it is created.
     * If the lookup and creation fails nullptr will be returned.
     *
     * Warning: Only call this function on the rendering thread, i.e.
     * from the QQuickItem::updatePaintedNode() overrides!
     *
     * Warning: The returned texture is owned by the surface, do not delete
     * it and do not allow any QSGNodes to take ownership!
     *
     * \param object the object to get the texture for
     * \param window the window to which the texture belongs to
     *
     * \return the texture
     */
    QSGTexture* getTexture(const QString& path, QQuickWindow* window) const;

    /**
     * Get the image for the path.
     *
     * Can be used to supply the image path to e.g. QImage.
     * The returned path points to a QResource component and as such is
     * only compatible with Qt's components.
     *
     * \param object the object to get the image for
     *
     * \return the path
     */
    QImage getImage(const QString& path) const;

    /**
     * Get the url of the image for the object.
     *
     * Can be used to supply the image url for Image qml components.
     * The returned url may point to a QResource component and as such
     * is only compatible with Qt's components.
     * TODO: make the surface a ImageProvider instead.
     *
     * \param object the object to get the image for
     *
     * \return the url
     */
    QUrl getImageUrl(const QString& path) const;

    /**
     * Get the path of the banner.
     *
     * The path is an internal path, only usable to fetch the image
     * via WorldSurface::getImageUrl().
     */
    QString getBannerImagePath(const core::Banner* const banner) const;

signals:
    /**
     * Emitted when the name changes.
     */
    void nameChanged();

    /**
     * Emitted when the description changes.
     */
    void descriptionChanged();

    /**
     * Emitted when the tile-width changes.
     */
    void tileWidthChanged();

    /**
     * Emitted when the tile-height changes.
     */
    void tileHeightChanged();

    /**
     * Emitted when the tile-size changes.
     */
    void tileSizeChanged();

private:
    void parseHeader(const QByteArray& header);

    const QString path;
    QString name;
    QString description;
    core::World* world;

    std::unique_ptr<Storage> storage;

    int tileWidth;
    int tileHeight;
    QImage hexMask;
};

/**
 * Is the world-surface sane?.
 *
 * A "sane" world-surface can be loaded and activated without exceptions.
 *
 * \param path the path to the world-surface
 * \param world a *sane* world
 *
 * \return is the world-surface sane
 *
 * \see isWorldSane
 */
bool isWorldSurfaceSane(const QString& path, core::World* world);

} // namespace ui
} // namespace warmonger

#endif // W_UI_WORLD_SURFACE_H
