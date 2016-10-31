# Warmonger

Warmonger is (will be) a turn based strategy game inspired by
[Battle for Wesnoth](http://wesnoth.org) and some other strategy games mixed
with my own ideas.

The project is under development and is not functional yet. As it is a pet
project and I work on it in my free time, I have no idea when it will be ready
to use.

## Why?

Mainly because I want to experiment with my own ideas for the perfect
turn-based strategy game. Secondly I want to learn QML and this seemed
like a good way to do that.

## How?

Warmonger is written with moddability and hackability in mind from the very
first line of code. It should be very easy to create a new mod for it (kind-of
like M&B in this respect). Extending warmonger shouldn't require more than a
little `json` and `$IMAGE_EDITOR` knowledge. Maybe more of the latter.

Warmonger is a hexagon-based wargame, with deterministic battle outcomes with
both great strategic and tactical challenges (well this is the plan).
Don't expect anything fancy on the graphics-side, I focus on the content (and
I am _very_ lame with image editors anyway).

## Dependencies
* C++14 compliant C++ compiler
* boost 1.58 (boost::log and boost::optional)
* Qt 5.5 (QtCore and QtQuick)
* KF5Archive

Version numbers refer to the minimum supported version of each dependency.
