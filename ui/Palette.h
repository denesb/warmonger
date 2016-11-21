/** \file
 * Palette class.
 *
 * \copyright (C) 2015-2016 Botond Dénes
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

#ifndef W_UI_PALETTE_H
#define W_UI_PALETTE_H

#include <QObject>
#include <QPalette>

namespace warmonger {
namespace ui {

class Palette : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QColor window READ getWindowColor CONSTANT)
    Q_PROPERTY(QColor windowText READ getWindowTextColor CONSTANT)
    Q_PROPERTY(QColor base READ getBaseColor CONSTANT)
    Q_PROPERTY(QColor alternateBase READ getAlternateBaseColor CONSTANT)
    Q_PROPERTY(QColor toolTipBase READ getToolTipBaseColor CONSTANT)
    Q_PROPERTY(QColor toolTipText READ getToolTipTextColor CONSTANT)
    Q_PROPERTY(QColor text READ getTextColor CONSTANT)
    Q_PROPERTY(QColor button READ getButtonColor CONSTANT)
    Q_PROPERTY(QColor buttonText READ getButtonTextColor CONSTANT)
    Q_PROPERTY(QColor brightText READ getBrightTextColor CONSTANT)
    Q_PROPERTY(QColor light READ getLightColor CONSTANT)
    Q_PROPERTY(QColor midlight READ getMidlightColor CONSTANT)
    Q_PROPERTY(QColor dark READ getDarkColor CONSTANT)
    Q_PROPERTY(QColor mid READ getMidColor CONSTANT)
    Q_PROPERTY(QColor shadow READ getShadowColor CONSTANT)
    Q_PROPERTY(QColor highlight READ getHighlightColor CONSTANT)
    Q_PROPERTY(QColor highlightedText READ getHighlightedTextColor CONSTANT)
    Q_PROPERTY(QColor link READ getLinkColor CONSTANT)
    Q_PROPERTY(QColor linkVisited READ getLinkVisitedColor CONSTANT)

public:
    Palette(const QPalette& palette, QPalette::ColorGroup colorGroup, QObject* parent = nullptr);

    QColor getWindowColor() const
    {
        return this->palette.color(this->colorGroup, QPalette::Window);
    }

    QColor getWindowTextColor() const
    {
        return this->palette.color(this->colorGroup, QPalette::WindowText);
    }

    QColor getBaseColor() const
    {
        return this->palette.color(this->colorGroup, QPalette::Base);
    }

    QColor getAlternateBaseColor() const
    {
        return this->palette.color(this->colorGroup, QPalette::AlternateBase);
    }

    QColor getToolTipBaseColor() const
    {
        return this->palette.color(this->colorGroup, QPalette::ToolTipBase);
    }

    QColor getToolTipTextColor() const
    {
        return this->palette.color(this->colorGroup, QPalette::ToolTipText);
    }

    QColor getTextColor() const
    {
        return this->palette.color(this->colorGroup, QPalette::Text);
    }

    QColor getButtonColor() const
    {
        return this->palette.color(this->colorGroup, QPalette::Button);
    }

    QColor getButtonTextColor() const
    {
        return this->palette.color(this->colorGroup, QPalette::ButtonText);
    }

    QColor getBrightTextColor() const
    {
        return this->palette.color(this->colorGroup, QPalette::BrightText);
    }

    QColor getLightColor() const
    {
        return this->palette.color(this->colorGroup, QPalette::Light);
    }

    QColor getMidlightColor() const
    {
        return this->palette.color(this->colorGroup, QPalette::Midlight);
    }

    QColor getDarkColor() const
    {
        return this->palette.color(this->colorGroup, QPalette::Dark);
    }

    QColor getMidColor() const
    {
        return this->palette.color(this->colorGroup, QPalette::Mid);
    }

    QColor getShadowColor() const
    {
        return this->palette.color(this->colorGroup, QPalette::Shadow);
    }

    QColor getHighlightColor() const
    {
        return this->palette.color(this->colorGroup, QPalette::Highlight);
    }

    QColor getHighlightedTextColor() const
    {
        return this->palette.color(this->colorGroup, QPalette::HighlightedText);
    }

    QColor getLinkColor() const
    {
        return this->palette.color(this->colorGroup, QPalette::Link);
    }

    QColor getLinkVisitedColor() const
    {
        return this->palette.color(this->colorGroup, QPalette::LinkVisited);
    }

private:
    QPalette palette;
    QPalette::ColorGroup colorGroup;
};

} // namespace ui
} // namespace warmonger

#endif // W_UI_PALETTE_H
