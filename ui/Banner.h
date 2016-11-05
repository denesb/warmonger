#ifndef UI_BANNER_H
#define UI_BANNER_H

#include <QColor>
#include <QQuickPaintedItem>

#include "core/Banner.h"
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
    Q_PROPERTY(QColor primaryColor READ getPrimaryColor WRITE setPrimaryColor NOTIFY primaryColorChanged)
    Q_PROPERTY(QColor secondaryColor READ fillColor WRITE setFillColor NOTIFY secondaryColorChanged)
    Q_PROPERTY(WorldSurface* worldSurface READ getWorldSurface WRITE setWorldSurface NOTIFY worldSurfaceChanged)

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
     *
     * \param banner the banner
     */
    void setBanner(core::Banner* banner);

    /**
     * Get the primary-color.
     *
     * \return the primary-color
     */
    const QColor& getPrimaryColor() const
    {
        return this->primaryColor;
    }

    /**
     * Set primary-color
     *
     * The secondary color is used as the foreground color of the banner.
     * If all conditions are given for drawing the banner, this will trigger a
     * redraw. If this was the missing piece it will trigger the first drawing.
     * Will emit the signal Banner::primaryColorChanged() if the
     * newly set value is different than the current one.
     *
     * \param primaryColor the primary-color
     */
    void setPrimaryColor(const QColor& primaryColor);

    /**
     * Get the world-surface used for drawing the banner.
     *
     * The Banner does not own the world-surface!
     *
     * \return the world-surface
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

    core::Banner* banner;
    QColor primaryColor;
    WorldSurface* worldSurface;
    QImage bannerImage;
};

} // namespace ui
} // namespace warmonger

#endif // UI_BANNER_H
