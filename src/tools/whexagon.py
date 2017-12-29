#! /usr/bin/env python3

import argparse
import math
from PIL import Image


# I don't normally align lines but here it helps comparing the colors.
BORDER_PIXEL      = (0x00, 0x00, 0x00, 0xff)
MIDDLE_PIXEL      = (0xff, 0xff, 0xff, 0xff)
UPPER_LEFT_PIXEL  = (0xff, 0xff, 0x00, 0xff)
UPPER_RIGHT_PIXEL = (0xff, 0x00, 0xff, 0xff)
LOWER_LEFT_PIXEL  = (0x00, 0xff, 0x00, 0xff)
LOWER_RIGHT_PIXEL = (0x00, 0x00, 0xff, 0xff)


class HexagonTemplateImage:
    def __init__(self, width, height):
        self.width = width
        self.height = height

        self._image = Image.new(mode="RGBA", size=(width, height), color=0)

    def set_at(self, x, y):
        return self._image.putpixel((x, y), BORDER_PIXEL)

    def write_to(self, filename):
        self._image.save(filename, format="PNG")


class HexagonMaskImage:
    def __init__(self, hexagon_template_image):
        template_image = hexagon_template_image._image
        size = template_image.size

        self._image = Image.new(mode="RGBA", size=size, color=0)

        height = size[1]

        self._paint_half_image(template_image,
                range(0, int(height / 2)),
                UPPER_LEFT_PIXEL,
                UPPER_RIGHT_PIXEL)

        self._paint_half_image(template_image,
                range(int(height / 2), height),
                LOWER_LEFT_PIXEL,
                LOWER_RIGHT_PIXEL)

    def _paint_half_image(self, template_image, y_range, left_pixel, right_pixel):
        width = template_image.size[0]

        for y in y_range:
            left_mask_pixel = left_pixel
            right_mask_pixel = right_pixel
            for x in range(0, int(width / 2)):
                pixel = template_image.getpixel((x, y))

                if pixel == BORDER_PIXEL:
                    left_mask_pixel = MIDDLE_PIXEL
                    right_mask_pixel = MIDDLE_PIXEL

                self._image.putpixel((x, y), left_mask_pixel)
                self._image.putpixel((width - x - 1, y), right_mask_pixel)

    def write_to(self, filename):
        self._image.save(filename, format="PNG")


def draw_hexagon_image(image, side):
    vpad = int((image.height - side) / 2)

    # draw lateral sides
    for x in range(vpad, vpad + side):
        image.set_at(0, x)
        image.set_at(image.width - 1, x)

    # draw the tips
    image.set_at(int(image.width / 2 - 1), image.height - 1)
    image.set_at(int(image.width / 2), image.height - 1,)
    image.set_at(int(image.width / 2 - 1), 0)
    image.set_at(int(image.width / 2), 0)

    # draw slanted sides
    h_diff = vpad - 2
    w_diff = int(image.width / 2) - 2
    step_diff = w_diff / h_diff # how much we have to step sideways before we step up
    step_diff_floor = math.floor(step_diff)

    x_top_left = 1
    x_top_right = image.width - 2
    x_bottom_left = int(image.width / 2) - 2
    x_bottom_right = int(image.width / 2) + 1

    y_top = int(1 + h_diff)
    y_bottom = image.height - 2

    x_top_left_target = int(image.width / 2) - 2
    y_top_target = 1

    x_debt = 0

    # since all sides have exactly the same length it's enough to monitor just
    # one of them
    while x_top_left < x_top_left_target and y_top > y_top_target:
        for i in range(0, step_diff_floor):
            image.set_at(x_top_left + i, y_top)
            image.set_at(x_top_right - i, y_top)
            image.set_at(x_bottom_left - i, y_bottom)
            image.set_at(x_bottom_right + i, y_bottom)

        x_top_left += step_diff_floor
        x_top_right -= step_diff_floor
        x_bottom_left -= step_diff_floor
        x_bottom_right += step_diff_floor
        y_top -= 1
        y_bottom -= 1

        x_debt += step_diff - step_diff_floor

        if x_debt > 1:
            x_debt -= 1

            image.set_at(x_top_left, y_top)
            image.set_at(x_top_right, y_top)
            image.set_at(x_bottom_left, y_bottom)
            image.set_at(x_bottom_right, y_bottom)

            x_top_left += 1
            x_top_right -= 1
            x_bottom_left -= 1
            x_bottom_right += 1

            image.set_at(x_top_left, y_top)
            image.set_at(x_top_right, y_top)
            image.set_at(x_bottom_left, y_bottom)
            image.set_at(x_bottom_right, y_bottom)

    image.set_at(x_top_left, y_top)
    image.set_at(x_top_right, y_top)
    image.set_at(x_bottom_left, y_bottom)
    image.set_at(x_bottom_right, y_bottom)


def nearest_even(n):
    nf = math.floor(n)

    if nf % 2:
        return nf + 1
    else:
        return nf


def params_based_on_width(width):
    side = width / math.sqrt(3)
    height = 2 * side
    return height, side


def params_based_on_height(height):
    side = height / 2
    width = side * math.sqrt(3)
    return width, side


def params_based_on_side(side):
    width = side * math.sqrt(3)
    height = 2 * side
    return width, height


def draw_hexagon(template_filename, mask_filename, width=None, height=None, side=None):
    if width:
        height, side = params_based_on_width(width)
    elif height:
        width, side = params_based_on_height(height)
    elif side:
        width, height = params_based_on_side(side)
    else:
        pass # WAT?

    width, height, side = map(nearest_even, [width, height, side])

    print("Drawing hexagon with tile size {width} x {height} and approximate side of {side}".format(width=width, height=height, side=side))

    template_image = HexagonTemplateImage(width, height)

    draw_hexagon_image(template_image, side)

    if template_filename is not None:
        template_image.write_to(template_filename)

    if mask_filename is not None:
        mask_image = HexagonMaskImage(template_image)
        mask_image.write_to(mask_filename)


if __name__ == '__main__':

    description = """\
Draw a pixel-perfect hexagon.
Specify one of the following:

     *          |
   *   *        |
 *       *      |
 *       * side | height
 *       *      |
   *   *        |
     *          |

 ---------
   width

Can draw a template image (only the borders are drawn) and/or a mask image for
determining whether a position is inside the hexagon or not.
For the former set the --template cmd line switch, for the latter set the
--mask switch to the filename you wish the result to be written to.
The format for the saved image is PNG, so use .png as the extension for the
outputs.
"""

    parser = argparse.ArgumentParser(
            description=description,
            formatter_class=argparse.RawDescriptionHelpFormatter)

    parser.add_argument("--width", help="The tile width", type=int)
    parser.add_argument("--height", help="The tile height", type=int)
    parser.add_argument("--side", help="The hexagon's side", type=int)
    parser.add_argument("--template", help="The filename where the template image will be written", required=False)
    parser.add_argument("--mask", help="The filename where the mask image will be written", required=False)

    args = parser.parse_args()

    if args.template is None and args.mask is None:
        parser.error("At least one output (--template or --mask) has to be specified")

    if args.width is not None:
        if args.width <= 0:
            parser.error("The width has to be a positive integer")
        elif args.height is not None or args.side is not None:
            parser.error("Only one of the options can be specified")
        else:
            draw_hexagon(args.template, args.mask, width=args.width)
    elif args.height is not None:
        if args.height <= 0:
            parser.error("The height has to be a positive integer")
        elif args.width is not None or args.side is not None:
            parser.error("Only one of the options can be specified")
        else:
            draw_hexagon(args.template, args.mask, height=args.height)
    elif args.side is not None:
        if args.side <= 0:
            parser.error("The side has to be a positive integer")
        elif args.width is not None or args.height is not None:
            parser.error("Only one of the options can be specified")
        else:
            draw_hexagon(args.template, args.mask, side=args.side)
    else:
        parser.error("At least one of the options must be specified")
