#! /usr/bin/env python3

import argparse
import math
from PIL import Image


class HexagonImage:
    def __init__(self, width, height):
        self.width = width
        self.height = height

        self._image = Image.new(mode="RGBA", size=(width, height), color=0)

    def set_at(self, x, y):
        return self._image.putpixel((x, y), (0x00, 0x00, 0x00, 0xff))

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

    x_depth = 0

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

        x_depth += step_diff - step_diff_floor

        if x_depth > 1:
            x_depth -= 1

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


def draw_hexagon(filename, width=None, height=None, side=None):
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

    image = HexagonImage(width, height)

    draw_hexagon_image(image, side)

    image.write_to(filename)


if __name__ == '__main__':

    description = """\
Draw a pixel-perfect hexagon.
Specify on of the following:

     *          |
   *   *        |
 *       *      |
 *       * side | height
 *       *      |
   *   *        |
     *          |

 ---------
   width
"""

    parser = argparse.ArgumentParser(
            description=description,
            formatter_class=argparse.RawDescriptionHelpFormatter)

    parser.add_argument("--width", help="The tile width", type=int)
    parser.add_argument("--height", help="The tile height", type=int)
    parser.add_argument("--side", help="The hexagon's side", type=int)
    parser.add_argument("--output", help="The filename where the image will be written", required=True)

    args = parser.parse_args()

    if args.width is not None:
        if args.width <= 0:
            parser.error("The width has to be a positive integer")
        elif args.height is not None or args.side is not None:
            parser.error("Only one of the options can be specified")
        else:
            draw_hexagon(args.output, width=args.width)
    elif args.height is not None:
        if args.height <= 0:
            parser.error("The height has to be a positive integer")
        elif args.width is not None or args.side is not None:
            parser.error("Only one of the options can be specified")
        else:
            draw_hexagon(args.output, height=args.height)
    elif args.side is not None:
        if args.side <= 0:
            parser.error("The side has to be a positive integer")
        elif args.width is not None or args.height is not None:
            parser.error("Only one of the options can be specified")
        else:
            draw_hexagon(args.output, side=args.side)
    else:
        parser.error("At least one of the options must be specified")
