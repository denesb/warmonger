#include "ui/Palette.h"

namespace warmonger {
namespace ui {

Palette::Palette(const QPalette& palette, QPalette::ColorGroup colorGroup, QObject* parent)
    : QObject(parent)
    , palette(palette)
    , colorGroup(colorGroup)
{
}

} // namespace ui
} // namespace warmonger
