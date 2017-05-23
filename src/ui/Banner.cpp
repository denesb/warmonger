/**
 * Copyright (C) 2015-2017 Botond Dénes
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

#include <QPainter>

#include "ui/Banner.h"

namespace warmonger {
namespace ui {

static QImage createBannerImage(WorldSurface* worldSurface, core::Banner* banner, const QColor& color);

Banner::Banner(QQuickItem* parent)
    : QQuickPaintedItem(parent)
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
    if (this->bannerImage.isNull())
        this->setFlags(0);
    else
        this->setFlags(QQuickItem::ItemHasContents);

    this->update();
}

static QImage createBannerImage(WorldSurface* worldSurface, core::Banner* banner, const QColor& color)
{
    if (worldSurface == nullptr || banner == nullptr || !color.isValid())
        return QImage();

    QImage image(worldSurface->getObjectImagePath(banner));

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
