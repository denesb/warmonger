/** \file
 * WorldSurface class.
 *
 * \copyright (C) 2015-2018 Botond Dénes
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
#include "ui/WorldSurfaceRules.h"
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
    Q_PROPERTY(int tileSize READ getTileSize CONSTANT)
    Q_PROPERTY(int gridSize READ getGridSize CONSTANT)

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
    WorldSurface(QString path, core::World* world, QObject* parent = nullptr);

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
     * Get the tile-size.
     *
     * \return the tile-size
     */
    int getTileSize() const
    {
        return this->tileSize;
    }

    int getGridSize() const
    {
        return this->gridSize;
    }

    WorldSurfaceRules::Type getRulesType() const
    {
        return this->rulesType;
    }

    WorldSurfaceRules& getRules()
    {
        return *this->rules;
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
     * Get the banner image.
     */
    QImage getBannerImage(const core::Banner& banner) const;

    /**
     * This surface's concrete color for the world's abstract one.
     */
    QColor colorFor(const core::Color& color) const;

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

private:
    void parseHeader(const QByteArray& header);

    const QString path;
    QString name;
    QString description;
    core::World* world;

    std::unique_ptr<Storage> storage;

    int tileSize;
    int gridSize;
    QImage hexMask;

    QString rulesEntryPoint;
    WorldSurfaceRules::Type rulesType;
    std::unique_ptr<WorldSurfaceRules> rules;

    std::unordered_map<QString, QString> banners;
    std::unordered_map<QString, QColor> colors;
};

/**
 * Find the world-surface.
 *
 * If `surface' is a relative or absolute path to an existing file its
 * canonical version is returned. Otherwise the surface is looked up in
 * the world's surfaces directory. If not found a null QString will be
 * returned.
 */
QString findWorldSurface(const QString& surface, const QString& worldPath);

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
