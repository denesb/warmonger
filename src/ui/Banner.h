/** \file
 * Banner class.
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

#ifndef UI_BANNER_H
#define UI_BANNER_H

#include <QQuickPaintedItem>

#include "ui/WorldSurface.h"

class QPainter;

namespace warmonger {
namespace ui {

/**
 * Displays a banner with the specific colors.
 *
 * The banner is painted with the primaryColor as the foreground and the
 * secondaryColor as the background. The QQuickPaintedItem's fillColor property
 * is used as the secondary-color. The secondaryColor promerty is merely an alias
 * to the fillColor. It needs the surface to get the actual banner image.
 */
class Banner : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(warmonger::core::Banner* banner READ getBanner WRITE setBanner NOTIFY bannerChanged)
    Q_PROPERTY(
        warmonger::core::Color* primaryColor READ getPrimaryColor WRITE setPrimaryColor NOTIFY primaryColorChanged)
    Q_PROPERTY(warmonger::core::Color* secondaryColor READ getSecondaryColor WRITE setSecondaryColor NOTIFY
            secondaryColorChanged)
    Q_PROPERTY(
        warmonger::ui::WorldSurface* worldSurface READ getWorldSurface WRITE setWorldSurface NOTIFY worldSurfaceChanged)

public:
    /**
     * Constructs an empty banner object.
     */
    explicit Banner(QQuickItem* parent = nullptr);

    /**
     * Get the displayed banner.
     *
     * \return the banner
     */
    core::Banner* getBanner() const
    {
        return this->banner;
    }

    /**
     * Set the banner.
     *
     * The banner identifies the banner to be drawn.
     * If all conditions are given for drawing the banner, this will trigger a
     * redraw. If this was the missing piece it will trigger the first drawing.
     * The Banner does not assume ownership of the banner!
     * Will emit the signal Banner::bannerChanged() if the newly set value is
     * different than the current one.
     */
    void setBanner(core::Banner* banner);

    core::Color* getPrimaryColor() const
    {
        return this->primaryColor;
    }

    /**
     * Set primary-color
     *
     * The promary color is used as the foreground color of the banner.
     * If all conditions are given for drawing the banner, this will trigger a
     * redraw. If this was the missing piece it will trigger the first drawing.
     * Will emit the signal Banner::primaryColorChanged() if the
     * newly set value is different than the current one.
     */
    void setPrimaryColor(core::Color* primaryColor);

    core::Color* getSecondaryColor() const
    {
        return this->secondaryColor;
    }

    /**
     * Set secondary-color
     *
     * The secondary color is used as the background color of the banner.
     * If all conditions are given for drawing the banner, this will trigger a
     * redraw. If this was the missing piece it will trigger the first drawing.
     * Will emit the signal Banner::primaryColorChanged() if the
     * newly set value is different than the current one.
     */
    void setSecondaryColor(core::Color* secondaryColor);

    /**
     * Get the world-surface used for drawing the banner.
     *
     * The Banner does not own the world-surface!
     */
    WorldSurface* getWorldSurface() const
    {
        return this->worldSurface;
    }

    /**
     * Set the world-surface used for drawing the banner.
     *
     * If all conditions are given for drawing the banner, this will trigger a
     * redraw. If this was the missing piece it will trigger the first drawing.
     * The Banner does not assume ownership of the world-surface!
     * Will emit the signal Banner::worldSurfaceChanged() if the
     * newly set value is different than the current one.
     *
     * \param worldSurface the world-surface
     */
    void setWorldSurface(WorldSurface* worldSurface);

    /**
     * Do the actual painting.
     *
     * \see QuickPaintedItem::paint()
     */
    void paint(QPainter* painter) override;

signals:
    /**
     * Emitted when the banner changes.
     */
    void bannerChanged();

    /**
     * Emitted when the primary-color changes.
     */
    void primaryColorChanged();

    /**
     * Emitted when the secondary-color (fill-color) changes.
     */
    void secondaryColorChanged();

    /**
     * Emitted when the world-surface changes.
     */
    void worldSurfaceChanged();

private:
    void updateContent();

    core::Banner* banner = nullptr;
    core::Color* primaryColor = nullptr;
    core::Color* secondaryColor = nullptr;
    WorldSurface* worldSurface = nullptr;
    QImage bannerImage;
};

} // namespace ui
} // namespace warmonger

#endif // UI_BANNER_H
