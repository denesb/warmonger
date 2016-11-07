#include <QPainter>

#include "ui/Banner.h"

namespace warmonger {
namespace ui {

static QImage createBannerImage(WorldSurface* worldSurface, core::Banner* banner, const QColor& color);

Banner::Banner(QQuickItem* parent) : QQuickPaintedItem(parent)
{
}

void Banner::setBanner(core::Banner* banner)
{
    if (this->banner != banner)
    {
        this->banner = banner;

        this->updateContent();

        emit bannerChanged();
    }
}

void Banner::setPrimaryColor(const QColor& primaryColor)
{
    if (this->primaryColor != primaryColor)
    {
        this->primaryColor = primaryColor;

        this->updateContent();

        emit primaryColorChanged();
    }
}

void Banner::setWorldSurface(WorldSurface* worldSurface)
{
    if (this->worldSurface != worldSurface)
    {
        this->worldSurface = worldSurface;

        this->updateContent();

        emit worldSurfaceChanged();
    }
}

void Banner::paint(QPainter* painter)
{
    if (this->bannerImage.isNull())
        return;

    const QRect source(QPoint(0, 0), this->bannerImage.size());
    painter->drawImage(painter->window(), this->bannerImage, source);
}

void Banner::updateContent()
{
    this->bannerImage = createBannerImage(this->worldSurface, this->banner, this->primaryColor);
    if(this->bannerImage.isNull())
        this->setFlags(0);
    else
        this->setFlags(QQuickItem::ItemHasContents);

    this->update();
}

static QImage createBannerImage(WorldSurface* worldSurface, core::Banner* banner, const QColor& color)
{
    if (worldSurface == nullptr || banner == nullptr || !color.isValid())
        return QImage();

    QImage image(":" + worldSurface->getImageUrl(banner).path());

    const QRgb rgb = color.rgb();
    const QSize size = image.size();
    for (int y = 0; y < size.height(); ++y)
        for (int x = 0; x < size.width(); ++x)
        {
            QRgb pixel = image.pixel(x, y);
            int alpha = qAlpha(pixel);

            if (alpha == 0x00)
                continue;

            if (alpha == 0xff)
            {
                image.setPixel(x, y, rgb);
            }
            else
            {
                QColor c(rgb);
                c.setAlpha(alpha);

                image.setPixel(x, y, c.rgba());
            }
        }

    return image;
}

} // namespace ui
} // namespace warmonger
